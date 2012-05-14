#ifndef COMPETITIEPARAMETERS_H
#define COMPETITIEPARAMETERS_H

#include <string>

class CompetitieParameters
{
	public:

	/**
	 * Default constructor.
	*/
	CompetitieParameters();

	/**
	 * Copy constructor.
	*/
	CompetitieParameters(CompetitieParameters& original);

	/**
	 * Destructor.
	*/
	virtual ~CompetitieParameters();

	/**
	 * Assignment operator.
	*/
	CompetitieParameters& operator=(const CompetitieParameters& original);

	/**
	 * Geeft de inhoud van dit object weer op de gegeven uitvoerstream.
	*/
	virtual void dump(std::ostream* outputStream);

	/**
	 * De titel van de competitie, bijv. "Interne Competitie S.C. Lutjebroek"
	*/
	std::string naam;

	/**
	 * Aanvangsdatum van dit competitieseizoen.
	*/
	std::string startDatum;

	/**
	 * Dit stelt de eigenwaarde in van de speler op plaats 1 op de ranglijst.
	 * De overige spelers hebben een eigenwaarde van "1 + x - p" waarbij 'p' de plaats van die speler op de ranglijst is, en 'x' de eigenwaarde van de speler op de 1e plaats.
	 *
	 * Door de waarde van deze instelling te veranderen kan een competitieleider spelen met de relatieve waarde van bijvoorbeeld remise tegen een topspeler versus winnen van een zwakke speler.
	 */
	int hoogsteEigenwaarde;
	
	/**
	 * Het aantal rondes dat momenteel gespeeld is in de competitie.
	 *
	 * TODO: refactor dit zodat het onderdeel wordt van de ronde-uitslagen!
	 */
	int aantalRondes;
};

#endif // COMPETITIEPARAMETERS_H

