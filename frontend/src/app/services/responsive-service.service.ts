import {Injectable} from '@angular/core';
import {BreakpointObserver, Breakpoints, BreakpointState} from "@angular/cdk/layout";
import {BehaviorSubject, Observable} from "rxjs";

@Injectable({
  providedIn: 'root'
})
export class ResponsiveService {

  private _isLargeScreen: BehaviorSubject<boolean>;

  public get isLargeScreen$(): Observable<boolean> {
    return this._isLargeScreen.asObservable();
  }


  public get isLargeScreen(): boolean {
    return this._isLargeScreen.value;
  }

  constructor(private breakpointObserver: BreakpointObserver) {

    this._isLargeScreen = new BehaviorSubject<boolean>(false);

    this.breakpointObserver.observe([Breakpoints.Large, Breakpoints.XLarge])
      .subscribe((state: BreakpointState) => {
          this._isLargeScreen.next(state.matches);
        }
      )
  }
}
