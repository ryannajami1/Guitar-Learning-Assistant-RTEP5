import { Component } from '@angular/core';
import {ChordsService} from '../../../services/chords.service';

@Component({
  selector: 'app-response-time',
  standalone: true,
  imports: [],
  templateUrl: './response-time.component.html',
  styleUrl: './response-time.component.scss'
})
export class ResponseTimeComponent {

  constructor(public chordsService: ChordsService) {
  }
}
