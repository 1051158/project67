# Latency

Dit project biedt een set Python-scripts voor het analyseren van netwerkpakketopnames, specifiek gericht op het berekenen van latentie en pakketverlies tussen een zender en ontvanger. De analyse wordt uitgevoerd met behulp van de bibliotheken `pyshark`, `pandas` en `matplotlib`.

## Vereisten

Zorg ervoor dat je de volgende Python-bibliotheken hebt geïnstalleerd:

- pyshark
- pandas
- matplotlib

Je kunt ze installeren met pip:

```bash
pip install pyshark pandas matplotlib
```

## Projectstructuur

- `data/` - Directory waar de pakketopnamebestanden moeten worden geplaatst.
- `analyze.py` - Hoofdscript om de analyse uit te voeren.

## Gebruik

### Instellen van Pakketopnamebestanden

Plaats je pakketopnamebestanden in de directory `data/`. Zorg ervoor dat de bestanden als volgt zijn genoemd:

- `sender_capture.pcap` - Opnamebestand van de zender.
- `receiver_capture.pcap` - Opnamebestand van de ontvanger.

### De Analyse Uitvoeren

Het script `analyze.py` laadt de pakketopnamebestanden, berekent latentie en analyseert pakketverlies. Om het script uit te voeren, gebruik:

```bash
python analyze.py
```

### Script Overzicht

Het script voert de volgende stappen uit:

1. **Laden van Pakketten**:
   - De functie `load_packets` leest de pakketopnamebestanden en extraheert relevante gegevens, specifiek de tijdstempel en UDP-payload.

2. **Latentie Berekenen**:
   - De functie `calculate_latency` voegt de gegevens van de zender en ontvanger samen op basis van de UDP-payload en berekent de latentie als het verschil tussen de tijdstempels van de ontvanger en de zender.

3. **Pakketverlies Berekenen**:
   - De functie `calculate_packet_loss` identificeert pakketten die zijn verzonden maar niet zijn ontvangen en berekent het pakketverliespercentage.

4. **Statistische Analyse**:
   - Het script geeft een statistische analyse van de latentie, inclusief het gemiddelde, de mediaan en andere beschrijvende statistieken.
   - Het biedt ook een analyse van het pakketverlies, inclusief het totale aantal verzonden, ontvangen en verloren pakketten.

5. **Plot Latentie**:
   - Het script genereert een plot van de latentie in de tijd, inclusief een voortschrijdend gemiddelde om trends te visualiseren.

### Uitvoer

- **Latentie Statistieken**:
  - Beschrijvende statistieken van latentie (min, max, gemiddelde, standaarddeviatie, etc.).

- **Pakketverlies Analyse**:
  - Totaal aantal verzonden en ontvangen pakketten.
  - Totaal aantal verloren pakketten.
  - Pakketverliespercentage.

- **Latentie Plot**:
  - Een plot die latentie in de tijd toont en een voortschrijdend gemiddelde latentie voor betere visualisatie.

### Voorbeeld Uitvoer

```plaintext
Statistische Analyse van Latentie (ms):
count    1000.000000
mean       10.345678
std         2.123456
min         8.123456
25%         9.234567
50%        10.345678
75%        11.456789
max        12.567890
Name: latency, dtype: float64

Pakketverlies Analyse:
Totaal Verzonden Pakketten: 1000
Totaal Ontvangen Pakketten: 950
Totaal Verloren Pakketten: 50
Pakketverliespercentage: 5.00%

Gemiddelde Latentie: 10.345678 milliseconden
```

## Opmerkingen

- Zorg ervoor dat de pakketopnamebestanden correct zijn geplaatst in de `data/` directory voordat je het script uitvoert.
- Pas de bestandsnamen in het script aan als je bestanden andere namen hebben of zich in een andere directory bevinden.

Dit project biedt een robuust kader voor het analyseren van netwerkpakketopnames, met de focus op latentie en pakketverlies. Gebruik de meegeleverde functies en het script als uitgangspunt voor verdere analyse en aanpassing aan je specifieke vereisten.