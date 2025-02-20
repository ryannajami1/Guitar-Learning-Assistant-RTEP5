import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DesktopSideNavComponent } from './desktop-side-nav.component';

describe('DesktopSideNavComponent', () => {
  let component: DesktopSideNavComponent;
  let fixture: ComponentFixture<DesktopSideNavComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DesktopSideNavComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DesktopSideNavComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
