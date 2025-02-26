import {Component} from '@angular/core';

declare var Raphael: any;

@Component({
  selector: 'app-chord-schematic',
  standalone: true,
  imports: [],
  templateUrl: './chord-schematic.component.html',
  styleUrl: './chord-schematic.component.scss'
})
export class ChordSchematicComponent {

  private chordContainer: any;

  ngOnInit(): void {
    this.chordContainer = Raphael.chord('chord-container', this.getRandomGuitarChord());
    this.chordContainer.element.setSize(300, 300);
  }

  getRandomGuitarChord(): string {
    const notes = ['A', 'B', 'C', 'D', 'E', 'F', 'G'];
    const chordTypes = ['', 'maj', 'min', '7', 'maj7', 'min7', 'sus4', 'dim', 'aug'];

    const randomNote = notes[Math.floor(Math.random() * notes.length)];
    const randomChordType = chordTypes[Math.floor(Math.random() * chordTypes.length)];

    return `${randomNote} ${randomChordType}`;
  }

  reloadChord(): void {
    this.chordContainer.remove();
    this.chordContainer = Raphael.chord('chord-container', this.getRandomGuitarChord());
    this.chordContainer.element.setSize(200, 200);
  }
}
