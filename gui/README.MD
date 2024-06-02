# GUI
## Beschrijving
Met de GUI kunnen commando's gemakkelijk naar de camera module gestuurd worden doormiddel van knoppen.
Hiermee kan de camera bijvoorbeeld uit of aan gedaan worden, of de kwaliteit veranderd worden op afstand.

## Vereisten
- Node.js
- Node-RED

## Installatie
1. download de node-red-dashboard module als je die nog niet geïnstaleerd hebt en run Node-RED met de volgende commando's:
```sh
npm i node-red-dashboard
node-red
```
2. Open `http://localhost:1880` in een browser om in de node-red editor te komen.
3. Import de GUI flow in het project (GUI flow is hier te vinden in: `./gui/flows.json`).
4. Deploy het project en open `http://localhost:1880/ui` in de browser om de GUI te openen.

## Gebruik
Interacties:
* `Camera On`: Zet de camera aan.
* `Camera Off`: Zet de camera uit.
* `Color`: Schakel tussen kleur en zwart-wit beeld.
* `Quality`: verander de kwaliteit van het beeld in bitrate
