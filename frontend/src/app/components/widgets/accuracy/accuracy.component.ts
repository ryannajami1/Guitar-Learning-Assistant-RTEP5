import { Component } from '@angular/core';
import {ChordPlayedService} from '../../../services/chord-played.service';

@Component({
  selector: 'app-accuracy',
  standalone: true,
  imports: [],
  templateUrl: './accuracy.component.html',
  styleUrl: './accuracy.component.scss'
})
export class AccuracyComponent {
  constructor(public chordPlayedService: ChordPlayedService) {
  }
}
