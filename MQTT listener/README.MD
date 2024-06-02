# MQTT Listener

## Overzicht

Dit script maakt gebruik van de `paho.mqtt.client` bibliotheek om een MQTT-client te implementeren die verschillende commando's kan ontvangen en verwerken om een camerastream te beheren. De commando's worden ontvangen via MQTT-berichten en kunnen de camera in- of uitschakelen, de kleurmodus wijzigen en de bitrate instellen.

## Vereisten

- Python 3.x
- paho-mqtt (`pip install paho-mqtt`)

## Configuratie

Voordat u het script uitvoert, moet u enkele configuratie-instellingen aanpassen:

```python
mqtt_broker_host = ""  # Hostnaam of IP-adres van de MQTT-broker
mqtt_port = 1883  # Poort van de MQTT-broker
target_host = ""  # IP-adres van het apparaat waarnaar we streamen
target_port = 5000  # Poort waarop we streamen
starting_bitrate = 500  # Start bitrate voor de stream
username = ""  # Gebruikersnaam voor MQTT-authenticatie
password = ""  # Wachtwoord voor MQTT-authenticatie
topic = ""  # MQTT-topic om berichten te ontvangen
path_to_stream_exe = ""  # Pad naar de uitvoerbare stream
```

## Functies

### `on_connect`

Deze functie wordt aangeroepen wanneer de client verbinding maakt met de MQTT-broker. Het abonneert zich op het opgegeven topic.

### `on_message`

Deze functie wordt aangeroepen wanneer een bericht wordt ontvangen op het geabonneerde topic. Het verwerkt de volgende commando's:

- `cameraOn`: Start de camerastream als deze nog niet actief is.
- `cameraOff`: Stopt de camerastream als deze actief is.
- `toggleColor:<mode>`: Wijzigt de kleurmodus naar de opgegeven waarde (`mode`).
- `setBitrate:<bitrate>`: Stelt de bitrate in op de opgegeven waarde (`bitrate`).

### `on_publish`

Deze functie wordt aangeroepen wanneer gegevens zijn gepubliceerd naar de broker.

## Pipes

Het script maakt FIFO-pipes aan voor communicatie met de stream:

- `/tmp/bitrate_fifo`
- `/tmp/color_toggle_fifo`

## Uitvoering

1. Zorg ervoor dat de vereisten zijn geïnstalleerd.
2. Pas de configuratie-instellingen aan in het script.
3. Voer het script uit:

```sh
python main.py
```

Het script zal verbinding maken met de MQTT-broker en berichten op het opgegeven topic beginnen verwerken.

