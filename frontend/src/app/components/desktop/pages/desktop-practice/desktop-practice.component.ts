import {Component} from '@angular/core';
import {ChordSchematicComponent} from '../../../widgets/chord-schematic/chord-schematic.component';
import {TimerComponent} from '../../../widgets/timer/timer.component';
import {ButtonModule} from 'primeng/button';
import {ResponseTimeComponent} from '../../../widgets/response-time/response-time.component';
import {LastFiveChordsComponent} from '../../../widgets/last-five-chords/last-five-chords.component';
import {AccuracyComponent} from '../../../widgets/accuracy/accuracy.component';

@Component({
  selector: 'app-desktop-practice',
  standalone: true,
  imports: [ChordSchematicComponent, TimerComponent, ButtonModule, ResponseTimeComponent, LastFiveChordsComponent, AccuracyComponent],
  templateUrl: './desktop-practice.component.html',
  styleUrl: './desktop-practice.component.scss'
})
export class DesktopPracticeComponent {

  currentChord: string;

  constructor() {
    this.currentChord = this.getRandomGuitarChord();
  }

  timeOver(): void {
    this.currentChord = this.getRandomGuitarChord();
  }

  getRandomGuitarChord(): string {
    const notes = ['A', 'B', 'C', 'D', 'E', 'F', 'G'];
    const chordTypes = ['', 'maj', 'min', '7', 'maj7', 'min7', 'sus4', 'dim', 'aug'];

    const randomNote = notes[Math.floor(Math.random() * notes.length)];
    const randomChordType = chordTypes[Math.floor(Math.random() * chordTypes.length)];

    return `${randomNote} ${randomChordType}`;
  }

}
