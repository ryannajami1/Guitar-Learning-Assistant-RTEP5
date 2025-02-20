import {Routes} from '@angular/router';
import {DesktopHomeComponent} from './components/desktop/pages/desktop-home/desktop-home.component';
import {MobileHomeComponent} from './components/mobile/pages/mobile-home/mobile-home.component';
import {MobilePracticeComponent} from './components/mobile/pages/mobile-practice/mobile-practice.component';
import {MobileSettingsComponent} from './components/mobile/pages/mobile-settings/mobile-settings.component';
import {DesktopSettingsComponent} from './components/desktop/pages/desktop-settings/desktop-settings.component';
import {DesktopPracticeComponent} from './components/desktop/pages/desktop-practice/desktop-practice.component';
import {MobileComponent} from './components/mobile/mobile.component';
import {DesktopComponent} from './components/desktop/desktop.component';

export const mobileRoutes: Routes = [
  {
    path: '', component: MobileComponent, children: [
      {path: 'home', component: MobileHomeComponent},
      {path: 'practice', component: MobilePracticeComponent},
      {path: 'settings', component: MobileSettingsComponent},
    ]
  },
];

export const desktopRoutes: Routes = [
  {
    path: '', component: DesktopComponent, children: [
      {path: 'home', component: DesktopHomeComponent},
      {path: 'practice', component: DesktopPracticeComponent},
      {path: 'settings', component: DesktopSettingsComponent},
    ],
  },
]

export const routes: Routes = []
