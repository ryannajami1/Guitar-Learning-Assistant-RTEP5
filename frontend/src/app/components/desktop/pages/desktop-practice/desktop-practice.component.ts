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
import {ChordPlayedService} from '../../../../services/chord-played.service';

@Component({
  selector: 'app-desktop-practice',
  standalone: true,
  imports: [DialogModule, CorrectChordComponent, WrongChordComponent, TimeoutComponent, ChordSchematicComponent, TimerComponent, ButtonModule, ResponseTimeComponent, LastFiveChordsComponent, AccuracyComponent, ToggleSwitchModule, FormsModule],
  templateUrl: './desktop-practice.component.html',
  styleUrl: './desktop-practice.component.scss'
})
export class DesktopPracticeComponent {

  currentChord: string;
  showChord: boolean = true;
  showSettings: boolean = false;
  showCorrectChordPlayed: boolean = false;
  showWrongChordPlayed: boolean = false;
  showTimeout: boolean = false;

  playA: boolean = true;
  playB: boolean = true;
  playC: boolean = true;
  playD: boolean = true;
  playE: boolean = true;
  playF: boolean = true;
  playG: boolean = true;

  playStandard: boolean = true;
  playMaj: boolean = true;
  playMin: boolean = true;
  play7: boolean = true;
  playMaj7: boolean = true;
  playMin7: boolean = true;
  playSus4: boolean = true;
  playDim: boolean = true;
  playAug: boolean = true;


  constructor(public chordPlayedService: ChordPlayedService) {
    this.currentChord = this.getRandomGuitarChord();
  }

  timeOver(): void {
    this.currentChord = this.getRandomGuitarChord();
  }

  getRandomGuitarChord(): string {

    let notes = [];

    if (this.playA) {
      notes.push('A');
    }
    if (this.playB) {
      notes.push('B');
    }
    if (this.playC) {
      notes.push('C');
    }
    if (this.playD) {
      notes.push('D');
    }
    if (this.playE) {
      notes.push('E');
    }
    if (this.playF) {
      notes.push('F');
    }
    if (this.playG) {
      notes.push('G');
    }

    let chordTypes = [];

    if (this.playStandard) {
      chordTypes.push('');
    }
    if (this.playMaj) {
      chordTypes.push('maj');
    }
    if (this.playMin) {
      chordTypes.push('min');
    }
    if (this.play7) {
      chordTypes.push('7');
    }
    if (this.playMaj7) {
      chordTypes.push('maj7');
    }
    if (this.playMin7) {
      chordTypes.push('min7');
    }
    if (this.playSus4) {
      chordTypes.push('sus4');
    }
    if (this.playDim) {
      chordTypes.push('dim');
    }
    if (this.playAug) {
      chordTypes.push('aug');
    }


    const randomNote = notes[Math.floor(Math.random() * notes.length)];
    const randomChordType = chordTypes[Math.floor(Math.random() * chordTypes.length)];

    return `${randomNote} ${randomChordType}`;
  }

}
