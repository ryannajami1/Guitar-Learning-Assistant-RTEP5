import { Component } from '@angular/core';
import {ChordPlayedService} from '../../../services/chord-played.service';
import {CommonModule} from '@angular/common';
import {EChordStatus} from '../../../models/chord-status.enum';

@Component({
  selector: 'app-last-five-chords',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './last-five-chords.component.html',
  styleUrl: './last-five-chords.component.scss'
})
export class LastFiveChordsComponent {
  EChordStatus = EChordStatus;

  constructor(public chordPlayedService: ChordPlayedService) {
  }

}
