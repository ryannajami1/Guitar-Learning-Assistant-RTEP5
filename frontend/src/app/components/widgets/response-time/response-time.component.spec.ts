import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ResponseTimeComponent } from './response-time.component';

describe('ResponseTimeComponent', () => {
  let component: ResponseTimeComponent;
  let fixture: ComponentFixture<ResponseTimeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ResponseTimeComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ResponseTimeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
