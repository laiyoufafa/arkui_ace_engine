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
 * ObservedPropertyObjectPU
 *
 * all definitions in this file are framework internal
 * 
 * class that holds an actual property value of type T
 * uses its base class to manage subscribers to this
 * property.
*/

class ObservedPropertyObjectPU<T extends Object> extends ObservedPropertyObjectAbstractPU<T>
  implements ISinglePropertyChangeSubscriber<T> {

  private wrappedValue_: T;

  constructor(value: T, owningView: IPropertySubscriber, propertyName: PropertyInfo) {
    super(owningView, propertyName);
    this.setValueInternal(value);
  }

  aboutToBeDeleted(unsubscribeMe?: IPropertySubscriber) {
    this.unsubscribeFromOwningProperty();
    if (unsubscribeMe) {
      this.unlinkSuscriber(unsubscribeMe.id__());
    }
    super.aboutToBeDeleted();
  }

  // notification from ObservedObject value one of its
  // props has chnaged. Implies the ObservedProperty has changed
  // Note: this function gets called when in this case:
  //       thisProp.aObsObj.aProp = 47  a object prop gets changed
  // It is NOT called when
  //    thisProp.aObsObj = new ClassA
  hasChanged(newValue: T): void {
    // stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: hasChanged DO NO USE`);
    // this.notifyHasChanged(this.wrappedValue_);
    // this.notifyPropertryHasChangedPU();
  }

  /**
   * Called by a @Link - SynchedPropertyObjectTwoWay that uses this as sync peer when it has changed
   * @param eventSource 
   */
  syncPeerHasChanged(eventSource : ObservedPropertyAbstractPU<T>) {
    stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: syncPeerHasChanged peer '${eventSource.info()}'.`);
    this.notifyPropertryHasChangedPU();
  }

  /**
   * Wraped ObservedObject has changed
   * @param souceObject 
   * @param changedPropertyName 
   */
  public objectPropertyHasChangedPU(souceObject: ObservedObject<T>, changedPropertyName : string) {
    stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: \
        objectPropertyHasChangedPU: contained ObservedObject property '${changedPropertyName}' has changed.`)
    this.notifyPropertryHasChangedPU();
  }

  private unsubscribeFromOwningProperty() {
    if (this.wrappedValue_) {
      if (this.wrappedValue_ instanceof SubscribaleAbstract) {
        (this.wrappedValue_ as SubscribaleAbstract).removeOwningProperty(this);
      } else {
        ObservedObject.removeOwningProperty(this.wrappedValue_, this);
      }
    }
  }
  /*
    actually update this.wrappedValue_
    called needs to do value change check
    and also notify with this.aboutToChange();
  */
  private setValueInternal(newValue: T): boolean {
    if (typeof newValue !== 'object') {
      stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}'] new value is NOT an object. Application error. Ignoring set.`);
      return false;
    }

    this.unsubscribeFromOwningProperty();

    if (ObservedObject.IsObservedObject(newValue)) {
      stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}'] new value is an ObservedObject already`);
      ObservedObject.addOwningProperty(newValue, this);
      this.wrappedValue_ = newValue;
    } else if (newValue instanceof SubscribaleAbstract) {
      stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}'] new value is an SubscribaleAbstract, subscribiung to it.`);
      this.wrappedValue_ = newValue;
      (this.wrappedValue_ as unknown as SubscribaleAbstract).addOwningProperty(this);
    } else {
      stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}'] new value is an Object, needs to be wrapped in an ObservedObject.`);
      this.wrappedValue_ = ObservedObject.createNew(newValue, this);
    }
    return true;
  }

  public get(): T {
    stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: get`);
    this.notifyPropertryHasBeenReadPU();
    return this.wrappedValue_;
  }

  public getUnmonitored(): T {
    stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: getUnmonitored returns '${JSON.stringify(this.wrappedValue_)}' .`);
    // unmonitored get access , no call to otifyPropertyRead !
    return this.wrappedValue_;
  }

  public set(newValue: T): void {
    if (this.wrappedValue_ == newValue) {
      stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: set with unchanged value - ignoring.`);
      return;
    }
    stateMgmtConsole.debug(`ObservedPropertyObject[${this.id__()}, '${this.info() || "unknown"}']: set, changed`);
    this.setValueInternal(newValue);
    //this.notifyHasChanged(newValue);
    this.notifyPropertryHasChangedPU();
  }
}
