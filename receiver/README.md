# Receiver

## Overzicht

Dit script start een GStreamer pipeline die RTP video ontvangt op een opgegeven poort en deze weergeeft. Standaard wordt poort 5000 gebruikt, maar je kunt een andere poort opgeven als argument bij het uitvoeren van het script.

## Vereisten

- GStreamer 1.0 moet geïnstalleerd zijn.
- GStreamer plugins: `gstreamer1.0-plugins-base`, `gstreamer1.0-plugins-good`, `gstreamer1.0-plugins-bad`, `gstreamer1.0-plugins-ugly`, `gstreamer1.0-libav`.
- Toegang tot een terminal of shell.

## Installatie

Om GStreamer en de vereiste plugins te installeren, gebruik je de volgende commando's afhankelijk van je besturingssysteem.

### Ubuntu/Debian/Raspberry OS

```bash
sudo apt-get update
sudo apt-get install -y gstreamer1.0-tools gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav
```


## Gebruik

### Standaard poort (5000)

Om de pipeline te starten met de standaard poort 5000, voer je het script uit zonder argumenten:

```bash
./receiver.sh
```

### Aangepaste poort

Om de pipeline te starten met een specifieke poort, geef je de poort als argument op:

```bash
./receiver.sh <PORT_NUMBER>
```

Bijvoorbeeld, om de pipeline te starten op poort 6000:

```bash
./receiver.sh 6000
```

## Scriptuitleg

Het script doet het volgende:

1. Stelt de standaard poort in op 5000.
2. Controleert of er een argument is opgegeven en stelt de poort in op de opgegeven waarde als dat het geval is.
3. Definieert de GStreamer pipeline om RTP video te ontvangen en weer te geven.
4. Voert de GStreamer pipeline uit.

## Voorbeeld

Stel dat je het script `receiver.sh` hebt genoemd. Om de video op poort 7000 te ontvangen, voer je het volgende commando uit:

```bash
./receiver.sh 7000
```

## Opmerking

Zorg ervoor dat je uitvoerrechten hebt op het scriptbestand. Je kunt dit doen met het volgende commando:

```bash
chmod +x receiver.sh
```