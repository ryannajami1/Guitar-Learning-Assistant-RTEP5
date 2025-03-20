import { Injectable } from '@angular/core';
import {Observable, Subject} from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class WsService {

  SOCKET_URL: string = "ws://192.168.1.127:9001" ;// MOVE THIS TO ENVIRONMENT
  private websocket: WebSocket;
  private messages: Subject<any> = new Subject();

  constructor() {
    this.websocket = new WebSocket(this.SOCKET_URL);
    this.connect();
  }

  connect() {
    console.log("CONNECT")
    this.websocket.onmessage = (event) => {
      this.messages.next(JSON.parse(event.data));
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
