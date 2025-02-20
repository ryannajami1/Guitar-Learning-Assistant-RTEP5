import {Routes} from '@angular/router';
import {HomeComponent} from './components/pages/home/home.component';
import {PracticeComponent} from './components/pages/practice/practice.component';
import {SettingsComponent} from './components/pages/settings/settings.component';

export const routes: Routes = [
  {path: 'home', component: HomeComponent},
  {path: 'practice', component: PracticeComponent},
  {path: 'settings', component: SettingsComponent},
  // {path: '**', redirectTo: 'home'}
];
