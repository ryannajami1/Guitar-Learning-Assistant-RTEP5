import {Component} from '@angular/core';
import {ChordSchematicComponent} from '../../../widgets/chord-schematic/chord-schematic.component';
import {TimerComponent} from '../../../widgets/timer/timer.component';

@Component({
  selector: 'app-desktop-practice',
  standalone: true,
  imports: [ChordSchematicComponent, TimerComponent],
  templateUrl: './desktop-practice.component.html',
  styleUrl: './desktop-practice.component.scss'
})
export class DesktopPracticeComponent {

}
