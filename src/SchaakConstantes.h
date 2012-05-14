#ifndef SCHAAKCONSTANTES_H
#define SCHAAKCONSTANTES_H

/**
 * Constantes ter representatie van uitslagen van partijen
 */
enum PartijResultaat
{
	ZWART_WINT = 0,
	WIT_WINT = 1,
	REMISE = 2,
	VRIJE_RONDE = 3
};

/**
 * Waarde voor Speler-ID integers die 'niemand' betekent.
 */
const int GEEN_SPELER = -1;

const double WINST_FACTOR       = (double) 6 / 6;  // Deel van eigenwaarde tegenstander dat bij winst wordt toegekend
const double VERLIES_FACTOR     = (double) 1 / 6;  // Deel van eigenwaarde tegenstander dat bij verlies wordt toegekend
const double REMISE_FACTOR      = (double) 3 / 6;  // Deek van eigenwaarde tegenstander dat bij remise wordt toegekend
const double VRIJE_RONDE_FACTOR = (double) 4 / 6;  // Deel van eigen eigenwaarde dat bij vrije ronde (oneven) wordt toegekend
const double AFWEZIG_FACTOR     = (double) 2 / 6;  // Deel van eigen eigenwaarde dat bij incidentele afwezigheid wordt toegekend
const int    MAX_AFWEZIG        =              5;  // Maximaal aantal keer per seizoen dat afwezigheid punten oplevert

#endif // SCHAAKCONSTANTES_H

