import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ChordSchematicComponent } from './chord-schematic.component';

describe('ChordSchematicComponent', () => {
  let component: ChordSchematicComponent;
  let fixture: ComponentFixture<ChordSchematicComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ChordSchematicComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ChordSchematicComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
