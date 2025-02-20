import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MobileSettingsComponent } from './mobile-settings.component';

describe('MobileSettingsComponent', () => {
  let component: MobileSettingsComponent;
  let fixture: ComponentFixture<MobileSettingsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [MobileSettingsComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(MobileSettingsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
