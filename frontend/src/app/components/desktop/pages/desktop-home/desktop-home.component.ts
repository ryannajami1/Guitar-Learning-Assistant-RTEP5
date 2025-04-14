import {Component} from '@angular/core';
import {WsService} from '../../../../services/ws.service';

@Component({
  selector: 'app-desktop-home',
  standalone: true,
  imports: [],
  templateUrl: './desktop-home.component.html',
  styleUrl: './desktop-home.component.scss'
})
export class DesktopHomeComponent {

  constructor(private wsService: WsService) {
  }

  messages: any[] = [];
  newMessage: string = 'TEST';

  ngOnInit() {
    this.wsService.getMessages().subscribe((message) => {
      this.messages.push(message);
    });
  }

  sendMessage() {
    if (this.newMessage.trim()) {
      console.log('SEND MESSAGE')
      this.wsService.sendMessage({text: this.newMessage});
      // this.newMessage = '';
    }
  }

  getMessage() {
    this.wsService.getMessages().subscribe((event) => {
      console.log(event);
    })
  }
}
