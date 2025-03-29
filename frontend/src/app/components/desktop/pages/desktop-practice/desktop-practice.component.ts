import {Component} from '@angular/core';
import {ChordSchematicComponent} from '../../../widgets/chord-schematic/chord-schematic.component';
import {TimerComponent} from '../../../widgets/timer/timer.component';
import {ButtonModule} from 'primeng/button';
import {ResponseTimeComponent} from '../../../widgets/response-time/response-time.component';
import {LastFiveChordsComponent} from '../../../widgets/last-five-chords/last-five-chords.component';
import {AccuracyComponent} from '../../../widgets/accuracy/accuracy.component';
import {ToggleSwitchModule} from 'primeng/toggleswitch';
import {FormsModule} from '@angular/forms';
import {DialogModule} from 'primeng/dialog';
import {CorrectChordComponent} from '../../../widgets/correct-chord/correct-chord.component';
import {WrongChordComponent} from '../../../widgets/wrong-chord/wrong-chord.component';
import {TimeoutComponent} from '../../../widgets/timeout/timeout.component';
import {ChordsService} from '../../../../services/chords.service';
import {EChordStatus} from '../../../../models/chord-status.enum';

@Component({
  selector: 'app-desktop-practice',
  standalone: true,
  imports: [DialogModule, CorrectChordComponent, WrongChordComponent, TimeoutComponent, ChordSchematicComponent, TimerComponent, ButtonModule, ResponseTimeComponent, LastFiveChordsComponent, AccuracyComponent, ToggleSwitchModule, FormsModule],
  templateUrl: './desktop-practice.component.html',
  styleUrl: './desktop-practice.component.scss'
})
export class DesktopPracticeComponent {
  EChordStatus = EChordStatus;

  showChord: boolean = true;
  showSettings: boolean = false;

  constructor(public chordPlayedService: ChordsService) {
    this.chordPlayedService.currentChordToBePlayed = this.chordPlayedService.getRandomGuitarChord();
  }
}
