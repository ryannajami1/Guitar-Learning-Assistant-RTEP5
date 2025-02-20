import { Component } from '@angular/core';
import {RouterOutlet} from '@angular/router';
import {MobileBottomNavComponent} from './mobile-bottom-nav/mobile-bottom-nav.component';

@Component({
  selector: 'app-mobile',
  standalone: true,
  imports: [RouterOutlet, MobileBottomNavComponent],
  templateUrl: './mobile.component.html',
  styleUrl: './mobile.component.scss'
})
export class MobileComponent {

}
