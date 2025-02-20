import {Component} from '@angular/core';
import {RouterOutlet} from '@angular/router';
import {ButtonModule} from 'primeng/button';
import {CommonModule} from '@angular/common';
import {Router} from '@angular/router';
import {MobileBottomNavComponent} from './components/mobile/mobile-bottom-nav/mobile-bottom-nav.component';
import {ResponsiveService} from './services/responsive-service.service';
import {desktopRoutes, mobileRoutes} from './app.routes';
import {distinctUntilChanged, pairwise} from "rxjs";


@Component({
  selector: 'app-root',
  standalone: true,
  imports: [CommonModule, RouterOutlet, ButtonModule, MobileBottomNavComponent],
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent {

  constructor(private responsiveService: ResponsiveService,
              private router: Router) {

    if (this.responsiveService.isLargeScreen) {
      this.router.resetConfig(desktopRoutes);
    }

    this.responsiveService.isLargeScreen$
      .pipe(distinctUntilChanged())
      .subscribe(() => {
        if (this.responsiveService.isLargeScreen) {
          this.router.resetConfig(desktopRoutes);
        } else {
          this.router.resetConfig(mobileRoutes);
        }
      })

    this.responsiveService.isLargeScreen$
      .pipe(pairwise())
      .subscribe(([previous, now]) => {
        if (previous != now && now) {
          window.location.reload();
        } else {
          window.location.reload();
        }
      })
  }

}
