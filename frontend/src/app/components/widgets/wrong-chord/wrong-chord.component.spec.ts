import { ComponentFixture, TestBed } from '@angular/core/testing';

import { WrongChordComponent } from './wrong-chord.component';

describe('WrongChordComponent', () => {
  let component: WrongChordComponent;
  let fixture: ComponentFixture<WrongChordComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [WrongChordComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(WrongChordComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
