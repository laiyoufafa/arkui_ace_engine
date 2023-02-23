/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * SynchedPropertyObjectOneWayPU
 * implementatio  of @Prop decorated variables of type class object
 * 
 * all definitions in this file are framework internal
 * 
 */

/**
 * Initialisation scenarios:
 * -------------------------
 * 
 * 1 - no local initialization, source provided (its ObservedObject value)
 *     wrap the ObservedObject into an ObservedPropertyObjectPU
 *     deep copy the ObservedObject into localCopyObservedObject
 * 
 * 2 - local initialization, no source provided
 *     app transpiled code calls set
 *     leave source_ undefined
 *     no deep copy needed, but provided local init might need wrapping inside an ObservedObject to set to 
 *     localCopyObservedObject
 * 
 * 3  local initialization,  source provided (its ObservedObject value)
 *    current app transpiled code is not optional
 *    sets source in constrcutor, as in case 1
 *    calls set() to set the source value, but this will not deepcopy
 * 
 * Update scenarios:
 * -----------------
 * 
 * 1- assignment of a new Object value: this.aProp = new ClassA()
 *    rhs can be ObservedObject because of @Observed decoration or now
 *    notifyPropertryHasChangedPU
 * 
 * 2- local ObservedObject member property change
 *    objectPropertyHasChangedPU called, eventSource is the ObservedObject stored in localCopyObservedObject
 *    no need to copy, notifyPropertryHasChangedPU
 * 
 * 3- Rerender of the custom component triggered from the parent
 *    reset() is called (code generated by the transpiler), set the valow of source_ ,  if that causes a change will call syncPeerHasChanged
 *    syncPeerHasChanged need to deep copy the ObservedObject from source to localCopyObservedObject
 *    notifyPropertryHasChangedPU
 * 
 * 4- source_ ObservedObject member property change
 *     objectPropertyHasChangedPU called, eventSource is the ObservedObject stored source_.getUnmonitores
 *     notifyPropertryHasChangedPU
 */
class SynchedPropertyObjectOneWayPU<C extends Object>
  extends ObservedPropertyObjectAbstractPU<C> {

  private localCopyObservedObject: C;
  private source_: ObservedPropertyAbstract<C>;

  constructor(source: ObservedPropertyAbstract<C> | C,
    owningChildView: IPropertySubscriber,
    thisPropertyName: PropertyInfo) {
    super(owningChildView, thisPropertyName);

    if (source == undefined) {
      stateMgmtConsole.log(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: constructor @Prop source is undefined.`);
      this.source_ = undefined;
      return;
    }

    if ((typeof (source) === "object") && ("subscribeMe" in source)) {
      // code path for @(Local)StorageProp, the souce is a ObservedPropertyObject in aLocalStorage)
      this.source_ = source as ObservedPropertyAbstractPU<C>;
      // subscribe to receive value change updates from LocalStorage source property
      this.source_.subscribeMe(this);
    } else {
      // code path for @Prop
      if (!ObservedObject.IsObservedObject(source)) {
        stateMgmtConsole.warn(`@Prop ${this.info()}  Provided source object's class 
           lacks @Observed class decorator. Object property changes will not be observed.`);
      }
      stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: constructor @Prop wrapping source in a new ObservedPropertyObjectPU`);
      this.source_ = new ObservedPropertyObjectPU<C>(source as C, this, this.getSourceObservedPropertyFakeName());
    }

    this.resetLocalValue(this.source_.get(), /* needDeepCopy */ true);
    stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: constructor ready with localCopyObservedObject '${JSON.stringify(this.localCopyObservedObject)}'.`);
  }

  /*
  like a destructor, need to call this before deleting
  the property.
  */
  aboutToBeDeleted() {
    if (this.source_) {
      this.source_.unlinkSuscriber(this.id__());
      this.source_ = undefined;
    }
    super.aboutToBeDeleted();
  }

  private getSourceObservedPropertyFakeName() : string {
    return `${this.info()}_source`;
  }

  public syncPeerHasChanged(eventSource: ObservedPropertyAbstractPU<C>) {
    if (this.source_ == undefined) {
      stateMgmtConsole.warn(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: \
       syncPeerHasChanged peer '${eventSource ? eventSource.info() : "no eventSource info"}' but source_ undefned. Internal error.`);
      return;
    }

    if (this.source_ == eventSource) {
      // defensive programming: should always be the case!
      stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}]: syncPeerHasChanged(): Source '${eventSource.info()}' has changed'.`)
      if (this.resetLocalValue(this.source_.getUnmonitored(), /* needDeepCopy */ true)) {
          this.notifyPropertryHasChangedPU();
      }
    } else {
      stateMgmtConsole.warn(`SynchedPropertyObjectOneWayPU[${this.id__()}]: syncPeerHasChanged Unexpected situation. Ignorning event.`)
    }
  }

  /**
   * event emited by wrapped ObservedObject, when one of its property values changes
   * @param souceObject
   * @param changedPropertyName
   */
  public objectPropertyHasChangedPU(souceObject: ObservedObject<C>, changedPropertyName: string) {
    if (this.source_ && souceObject == this.source_.getUnmonitored()) {
      stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: \
      objectPropertyHasChangedPU: member property '${changedPropertyName}' of source ObservedObject '${this.source_.info()}' has changed.`);
      this.resetLocalValue(souceObject as C, /* needDeepCopy */ true);
    } else {
      stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: \
        objectPropertyHasChangedPU: contained ObservedObject member property '${changedPropertyName}' has changed.`);
    }
    this.notifyPropertryHasChangedPU();
  }

  public getUnmonitored(): C {
    stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: getUnmonitored returns '${JSON.stringify(this.localCopyObservedObject)}'.`);
    // unmonitored get access , no call to notifyPropertyRead !
    return this.localCopyObservedObject;
  }

  // get 'read through` from the ObservedObject
  public get(): C {
    stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: get returning ${JSON.stringify(this.localCopyObservedObject)}.`)
    this.notifyPropertryHasBeenReadPU()
    return this.localCopyObservedObject;
  }

  // assignment to local variable in the form of this.aProp = <object value>
  // set 'writes through` to the ObservedObject
  public set(newValue: C): void {
    if (this.localCopyObservedObject == newValue) {
      stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}IP, '${this.info() || "unknown"}']: set with unchanged value '${JSON.stringify(newValue)}'- ignoring.`);
      return;
    }

    stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: set to newValue: '${JSON.stringify(newValue)}'.`);

    if (this.resetLocalValue(newValue, /* needDeepCopy */ false)) {
      this.notifyPropertryHasChangedPU();
    }
  }


  public reset(sourceChangedValue: C): void {
    stateMgmtConsole.debug(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: reset from '${JSON.stringify(this.localCopyObservedObject)}' to '${JSON.stringify(sourceChangedValue)}'.`);
    if (this.source_ !== undefined) {
      // if set causes an actual change, then, ObservedPropertyObject source_ will call syncPeerHasChanged
      this.source_.set(sourceChangedValue);
    } else {
      stateMgmtConsole.error(`SynchedPropertyObjectOneWayPU[${this.id__()}, '${this.info() || "unknown"}']: reset from '${JSON.stringify(this.localCopyObservedObject)}' to '${JSON.stringify(sourceChangedValue)}' No source_. Internal error!`);
    }
  }

  /*
    unsubscribe from previous wrappped ObjectObject
    take a deep copy
    copied Object might already be an ObservedObject (e.g. becuse of @Observed decroator) or might be raw
    Therefore, conditionally wrap the object, then subscribe
    return value tue indicates localCopyObservedObject has been changed
  */
  private resetLocalValue(newObservedObjectValue: C, needDeepCopy : boolean): boolean {
    // note: We can not test for newObservedObjectValue == this.localCopyObservedObject
    // here because the object might still be the same, but some property of it has changed

    if (newObservedObjectValue !== undefined && typeof newObservedObjectValue !== "object") {
      stateMgmtConsole.error(`SynchedPropertyOneWayObjectPU[${this.id__()}]: setLocalValue new value must be an Object. Not setting.`)
      return false;
    }

    // unsubscribe from old wappedValue ObservedOject  
    ObservedObject.removeOwningProperty(this.localCopyObservedObject, this);


    if (newObservedObjectValue == undefined) {
      // case: newObservedObjectValue undefined
      this.localCopyObservedObject = undefined;
      return true;
    }

    // deep copy value 
    // needed whenever newObservedObjectValue comes from source
    // not needed on a local set (aka when called from set() method)
    let copy = needDeepCopy ? ObservedObject.GetDeepCopyOfObject(newObservedObjectValue) : newObservedObjectValue;

    if (ObservedObject.IsObservedObject(copy)) {
      // case: new ObservedObject
      this.localCopyObservedObject = copy;
      ObservedObject.addOwningProperty(this.localCopyObservedObject, this);
    } else {
      // wrap newObservedObjectValue raw object as ObservedObject and subscribe to it
      stateMgmtConsole.warn(`@Prop ${this.info()}  Provided source object's class \
          lacks @Observed class decorator. Object property changes will not be observed.`);
      this.localCopyObservedObject = ObservedObject.createNew(copy, this);
    }
    return true;
  }
}
