import { Injectable } from '@angular/core';
import {Observable, Subject} from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class WsService {

  socketUrl: string = "" ;

  private websocket: WebSocket;
  private messages: Subject<any> = new Subject();

  constructor() {
    const protocol = window.location.protocol === 'https:' ? 'wss' : 'ws';
    const hostname = window.location.hostname;
    this.socketUrl = `${protocol}://${hostname}:9000`
    this.websocket = new WebSocket(this.socketUrl);
    this.connect();
  }

  connect() {
    console.log("CONNECT @ " + this.socketUrl);

    this.websocket.onopen = (event) => {
      console.log('WebSocket connection established!');
    }

    this.websocket.onmessage = (event) => {
      console.log(event);
      console.log(event.data);
      this.messages.next(event.data);
    };

    this.websocket.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    this.websocket.onclose = () => {
      console.log("WebSocket connection closed");
    };
  }
  sendMessage(message: any) {
    if (this.websocket.readyState === WebSocket.OPEN) {
      let json = JSON.stringify(message)
      console.log(json)
      this.websocket.send(json);
    }
  }
  getMessages(): Observable<any> {
    return this.messages.asObservable();
  }
}
