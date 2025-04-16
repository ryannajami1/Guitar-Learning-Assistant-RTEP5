import {Component, ElementRef, EventEmitter, Input, Output, Renderer2, ViewChild} from '@angular/core';
import {ProgressBar} from 'primeng/progressbar';
import {ChordsService} from '../../../services/chords.service';

@Component({
  selector: 'app-timer',
  standalone: true,
  imports: [ProgressBar],
  templateUrl: './timer.component.html',
  styleUrl: './timer.component.scss'
})
export class TimerComponent {
  @ViewChild("loadingBarActive") loadingBarActive: ElementRef | undefined;

  constructor(private renderer: Renderer2,
              public chordService: ChordsService) {

    this.chordService.timerRunning$.subscribe((value) => {
      if (value) {
        this.renderer.setStyle(this.loadingBarActive?.nativeElement, 'transition', `width ${this.chordService.timerStartValueMs/1000}s linear`);
        this.renderer.addClass(this.loadingBarActive?.nativeElement, 'shrink');
      } else {
        this.renderer.setStyle(this.loadingBarActive?.nativeElement, 'transition', '0s');
        this.renderer.removeClass(this.loadingBarActive?.nativeElement, 'shrink');
      }
    })
  }
}
