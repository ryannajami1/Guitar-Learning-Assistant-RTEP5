import { ComponentFixture, TestBed } from '@angular/core/testing';

import { LastFiveChordsComponent } from './last-five-chords.component';

describe('LastFiveChordsComponent', () => {
  let component: LastFiveChordsComponent;
  let fixture: ComponentFixture<LastFiveChordsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [LastFiveChordsComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(LastFiveChordsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
