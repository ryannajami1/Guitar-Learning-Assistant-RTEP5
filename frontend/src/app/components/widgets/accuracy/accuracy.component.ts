import { Component } from '@angular/core';
import {ChordsService} from '../../../services/chords.service';

@Component({
  selector: 'app-accuracy',
  standalone: true,
  imports: [],
  templateUrl: './accuracy.component.html',
  styleUrl: './accuracy.component.scss'
})
export class AccuracyComponent {
  constructor(public chordsService: ChordsService) {
  }
}
