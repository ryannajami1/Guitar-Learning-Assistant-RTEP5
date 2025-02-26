import {Component, Input} from '@angular/core';

declare var Raphael: any;

@Component({
  selector: 'app-chord-schematic',
  standalone: true,
  imports: [],
  templateUrl: './chord-schematic.component.html',
  styleUrl: './chord-schematic.component.scss'
})
export class ChordSchematicComponent {
  @Input() currentChord: string = '';

  private chordContainer: any;

  ngOnChanges(): void {
    this.reloadChord();
  }

  reloadChord(): void {
    if (this.chordContainer) {
      this.chordContainer.remove();
    }
    this.chordContainer = Raphael.chord('chord-container', this.currentChord);
    this.chordContainer.element.setSize(300, 300);
  }

}
