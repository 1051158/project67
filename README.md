# Project78

## Introductie

Dit is onze GitHub-repository met alle code voor Project 78. Dit project omvat verschillende modules voor het afhandelen van MQTT-berichten, een grafische gebruikersinterface (GUI), latentiebeheer en video streamen. Voor instructies over de specifieke onderdelen kun je naar de betreffende map navigeren.

## Installatie

Volg de onderstaande stappen om het project op uw lokale machine op te zetten:

1. **Repository Klonen**
    ```bash
    git clone [repository_url]
    cd project78-main
    ```

2. **Python Vereisten Installeren**
    Voor de MQTT Listener:
    ```bash
    pip install -r MQTT\ listener/requirements.txt
    ```
   Voor de latency analyze:
    ```bash
    pip install -r latency/requirements.txt
    ```

### Uitvoeren

Om een stream werkend te krijgen, heb je de `MQTT listener` en `stream` folders nodig op je camera module. Er wordt van uitgegaan dat dit een Linux-apparaat is. Nadat je het `stream.cpp` bestand hebt gecompileerd en de MQTT-listener correct hebt geconfigureerd, kun je het programma starten door de listener aan te zetten met:

```bash
python main.py
```

Je kan nu op de ontvangende computer de receive script uitvoeren om video te ontvangen:
```bash
cd receiver
chmod +x receiver.sh
./receiver.sh
```