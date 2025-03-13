import { ComponentFixture, TestBed } from '@angular/core/testing';

import { TimeoutComponent } from './timeout.component';

describe('TimeoutComponent', () => {
  let component: TimeoutComponent;
  let fixture: ComponentFixture<TimeoutComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [TimeoutComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(TimeoutComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
