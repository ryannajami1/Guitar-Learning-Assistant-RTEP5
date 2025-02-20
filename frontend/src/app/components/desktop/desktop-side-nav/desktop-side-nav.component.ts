import { Component } from '@angular/core';
import {RouterLink, RouterLinkActive} from '@angular/router';
import {ImageModule} from 'primeng/image';

@Component({
  selector: 'app-desktop-side-nav',
  standalone: true,
  imports: [RouterLink, RouterLinkActive, ImageModule],
  templateUrl: './desktop-side-nav.component.html',
  styleUrl: './desktop-side-nav.component.scss'
})
export class DesktopSideNavComponent {

}
