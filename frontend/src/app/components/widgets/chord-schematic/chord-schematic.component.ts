import {Component, EventEmitter, Input, Output} from '@angular/core';
import {CommonModule, NgIf} from '@angular/common';
import {ButtonModule} from 'primeng/button';

declare var Raphael: any;

@Component({
  selector: 'app-chord-schematic',
  standalone: true,
  imports: [CommonModule, NgIf, ButtonModule],
  templateUrl: './chord-schematic.component.html',
  styleUrl: './chord-schematic.component.scss'
})
export class ChordSchematicComponent {
  @Input() currentChord: string = '';
  @Output() currentChordChange: EventEmitter<void> = new EventEmitter<void>();
  @Input() showChord: boolean = true;

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
