import { Component } from '@angular/core';
import {RouterOutlet} from '@angular/router';
import {DesktopSideNavComponent} from './desktop-side-nav/desktop-side-nav.component';

@Component({
  selector: 'app-desktop',
  standalone: true,
  imports: [RouterOutlet, DesktopSideNavComponent],
  templateUrl: './desktop.component.html',
  styleUrl: './desktop.component.scss'
})
export class DesktopComponent {

}
