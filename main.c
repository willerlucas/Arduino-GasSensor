	const int gas_sensor = A0;
	const int porta_alarme = 6;
	const int porta_motor = 9;

	// ***********************************************
	// Inicio da biblioteca de logica fuzzy
	//************************************************
	/*
	Biblioteca eFLL (Embedded Fuzzy Logic) para a plataforma Tinkercad
	*/

	//#include <iostream>
	#include <stdlib.h>

	using namespace std;

	// CONSTANTES
	#define EPS 1.0E-3

	// CONSTANTES
	#define OP_AND 1
	#define OP_OR 2
	#define MODE_FS 1
	#define MODE_FS_FS 2
	#define MODE_FS_FRA 3
	#define MODE_FRA_FRA 4

	// Estrutura de uma lista de FuzzySet

	class FuzzySet {
		public:
			// CONSTRUTORES
			FuzzySet();
			FuzzySet(float a, float b, float c, float d);
			// M�TODOS P�BLICOS
			float getPointA();
			float getPointB();
			float getPointC();
			float getPointD();
			bool calculatePertinence(float crispValue);
			void setPertinence(float pertinence);
			float getPertinence();
			void reset();

		private:
			// VARI�VEIS
			float a;
			float b;
			float c;
			float d;
			float pertinence;
	};

	struct fuzzySetArray{
		FuzzySet* fuzzySet;
		fuzzySetArray* next;
	};


	class FuzzyIO {
		public:
			// CONSTRUTORES
			FuzzyIO();
			FuzzyIO(int index);
			// DESTRUTOR
			~FuzzyIO();
			// M�TODOS P�BLICOS
			int getIndex();
			void setCrispInput(float crispInput);
			float getCrispInput();
			bool addFuzzySet(FuzzySet* fuzzySet);
			void resetFuzzySets();

		protected:
			// VARI�VEIS PROTEGIDAS
			int index;
			float crispInput;
			fuzzySetArray* fuzzySets;
			fuzzySetArray* fuzzySetsCursor;
			// M�TODOS PROTEGIDOS
			void cleanFuzzySets(fuzzySetArray* aux);
	};


	class FuzzyInput : public FuzzyIO {
		public:
			// CONSTRUTORES
	        FuzzyInput();
			FuzzyInput(int index);
			// DESTRUTOR
			~FuzzyInput();
			// M�TODOS P�BLICOS
			bool calculateFuzzySetPertinences();
	};

	// Estrutura de uma matriz de fuzzyInputArray
	struct fuzzyInputArray{
		FuzzyInput* fuzzyInput;
		fuzzyInputArray* next;
	};

	// Estrutura de uma lista para guardar os pontos
	struct pointsArray{
		pointsArray* previous;
		float point;
		float pertinence;
		pointsArray* next;
	};


	class FuzzyComposition{
		public:
			// CONSTRUTORES
			FuzzyComposition();
			// DESTRUTOR
			~FuzzyComposition();
			// M�TODOS P�BLICOS
			bool addPoint(float point, float pertinence);
			bool checkPoint(float point, float pertinence);
			bool build();
			float avaliate();
			bool empty();

		private:
			// VARI�VEIS PRIVADAS
			pointsArray* pointsCursor;
			pointsArray* points;

			// M�TODOS PRIVADOS
			void cleanPoints(pointsArray* aux);
			bool rebuild(pointsArray* aSegmentBegin, pointsArray* aSegmentEnd, pointsArray* bSegmentBegin, pointsArray* bSegmentEnd);
			bool rmvPoint(pointsArray* point);
	};


	class FuzzyOutput : public FuzzyIO {
		public:
			// CONSTRUTORES
			FuzzyOutput();
			FuzzyOutput(int index);
			// DESTRUTOR
			~FuzzyOutput();
			// M�TODOS P�BLICOS
			bool truncate();
			float getCrispOutput();
			bool order();

		private:
			// VARI�VEIS PRIVADAS
			FuzzyComposition fuzzyComposition;
			// M�TODOS PRIVADOS
			bool swap(fuzzySetArray* fuzzySetA, fuzzySetArray* fuzzySetB);
	};

	// Estrutura de uma matriz de fuzzyOutputArray
	struct fuzzyOutputArray{
		FuzzyOutput* fuzzyOutput;
		fuzzyOutputArray* next;
	};

	class FuzzyRuleAntecedent {
		public:
			// CONSTRUTORES
			FuzzyRuleAntecedent();
			// M�TODOS P�BLICOS
			bool joinSingle(FuzzySet* fuzzySet);
			bool joinWithAND(FuzzySet* fuzzySet1, FuzzySet* fuzzySet2);
			bool joinWithOR(FuzzySet* fuzzySet1, FuzzySet* fuzzySet2);
			bool joinWithAND(FuzzySet* fuzzySet, FuzzyRuleAntecedent* fuzzyRuleAntecedent);
			bool joinWithAND(FuzzyRuleAntecedent* fuzzyRuleAntecedent, FuzzySet* fuzzySet);
			bool joinWithOR(FuzzySet* fuzzySet, FuzzyRuleAntecedent* fuzzyRuleAntecedent);
			bool joinWithOR(FuzzyRuleAntecedent* fuzzyRuleAntecedent, FuzzySet* fuzzySet);
			bool joinWithAND(FuzzyRuleAntecedent* fuzzyRuleAntecedent1, FuzzyRuleAntecedent* fuzzyRuleAntecedent2);
			bool joinWithOR(FuzzyRuleAntecedent* fuzzyRuleAntecedent1, FuzzyRuleAntecedent* fuzzyRuleAntecedent2);
			float evaluate();

		private:
			// VARI�VEIS PRIVADAS
			int op; // operador l�gico
			int mode;
			FuzzySet* fuzzySet1;
			FuzzySet* fuzzySet2;
			FuzzyRuleAntecedent* fuzzyRuleAntecedent1;
			FuzzyRuleAntecedent* fuzzyRuleAntecedent2;
	};

	// Estrutura de uma lista de FuzzySet
	struct fuzzySetOutputArray{
		FuzzySet* fuzzySet;
		fuzzySetOutputArray* next;
	};


	class FuzzyRuleConsequent {
		public:
			// CONSTRUTORES
			FuzzyRuleConsequent();
			// DESTRUTOR
			~FuzzyRuleConsequent();
			// M�TODOS P�BLICOS
			bool addOutput(FuzzySet* fuzzySet);
			bool evaluate(float power);

		private:
			// VARI�VEIS PRIVADAS
			fuzzySetOutputArray* fuzzySetOutputsCursor;
			fuzzySetOutputArray* fuzzySetOutputs;
			// M�TODOS PRIVADOS
			void cleanFuzzySets(fuzzySetOutputArray* aux);
	};


	class FuzzyRule {
		public:
			// CONSTRUTORES
			FuzzyRule();
			FuzzyRule(int index, FuzzyRuleAntecedent* fuzzyRuleAntecedent, FuzzyRuleConsequent* fuzzyRuleConsequent);
			// M�TODOS P�BLICOS
			int getIndex();
			bool evaluateExpression();
			bool isFired();

		private:
			// VARI�VEIS PRIVADAS
			int index;
			bool fired;
			FuzzyRuleAntecedent* fuzzyRuleAntecedent;
			FuzzyRuleConsequent* fuzzyRuleConsequent;
	};

	// Estrutura de uma lista de FuzzyRule
	struct fuzzyRuleArray{
		FuzzyRule* fuzzyRule;
		fuzzyRuleArray* next;
	};


	class Fuzzy {
		public:
			// CONSTRUTORES
			Fuzzy();
			// DESTRUTOR
			~Fuzzy();
			// M�TODOS P�BLICOS
			bool addFuzzyInput(FuzzyInput* fuzzyInput);
			bool addFuzzyOutput(FuzzyOutput* fuzzyOutput);
			bool addFuzzyRule(FuzzyRule* fuzzyRule);
			bool setInput(int fuzzyInputIndex, float crispValue);
			bool fuzzify();
			bool isFiredRule(int fuzzyRuleIndex);
			float defuzzify(int fuzzyOutputIndex);

		private:
			// VARI�VEIS PRIVADAS
			// ponteiros para gerenciar os arrays de FuzzyInput
			fuzzyInputArray* fuzzyInputsCursor;
			fuzzyInputArray* fuzzyInputs;
			// ponteiros para gerenciar os arrays de FuzzyOutput
			fuzzyOutputArray* fuzzyOutputsCursor;
			fuzzyOutputArray* fuzzyOutputs;
			// ponteiros para gerenciar os arrays de FuzzyRule
			fuzzyRuleArray* fuzzyRulesCursor;
			fuzzyRuleArray* fuzzyRules;

			// M�TODOS PRIVADOS
			void cleanFuzzyInputs(fuzzyInputArray* aux);
			void cleanFuzzyOutputs(fuzzyOutputArray* aux);
			void cleanFuzzyRules(fuzzyRuleArray* aux);
	};



	// Implementa��o das classes

	FuzzySet::FuzzySet(){
	}

	FuzzySet::FuzzySet(float a, float b, float c, float d){
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->pertinence = 0.0;
	}

	float FuzzySet::getPointA(){
		return this->a;
	}

	float FuzzySet::getPointB(){
		return this->b;
	}

	float FuzzySet::getPointC(){
		return this->c;
	}

	float FuzzySet::getPointD(){
		return this->d;
	}

	bool FuzzySet::calculatePertinence(float crispValue){
		float slope;

		if (crispValue < this->a){
			if (this->a == this->b && this->b != this->c && this->c != this->d){
				this->pertinence = 1.0;
			}else{
				this->pertinence = 0.0;
			}
		}else if (crispValue >= this->a && crispValue < this->b){
			slope = 1.0 / (this->b - this->a);
			this->pertinence = slope * (crispValue - this->b) + 1.0;
		}else if (crispValue >= this->b && crispValue <= this->c){
			this->pertinence = 1.0;
		}else if (crispValue > this->c && crispValue <= this->d){
			slope = 1.0 / (this->c - this->d);
			this->pertinence = slope * (crispValue - this->c) + 1.0;
		}else if (crispValue > this->d){
			if (this->c == this->d && this->c != this->b && this->b != this->a){
				this->pertinence = 1.0;
			}else{
				this->pertinence = 0.0;
			}
		}
		return true;
	}

	void FuzzySet::setPertinence(float pertinence){
		if(this->pertinence < pertinence){
			this->pertinence = pertinence;
		}
	}

	float FuzzySet::getPertinence(){
		return this->pertinence;
	}

	void FuzzySet::reset(){
		this->pertinence = 0.0;
	}

	// CONSTRUTORES
	FuzzyRuleConsequent::FuzzyRuleConsequent(){
		this->fuzzySetOutputs = NULL;
		this->fuzzySetOutputsCursor = NULL;
	}

	// DESTRUTOR
	FuzzyRuleConsequent::~FuzzyRuleConsequent(){
		this->cleanFuzzySets(this->fuzzySetOutputs);
	}

	// M�TODOS P�BLICOS
	bool FuzzyRuleConsequent::addOutput(FuzzySet* fuzzySet){
		fuzzySetOutputArray *aux;
		// Alocando espa�o na mem�ria
		if((aux = (fuzzySetOutputArray *) malloc(sizeof(fuzzySetOutputArray))) == NULL){
			return false;
		}
		aux->fuzzySet 	= fuzzySet;
		aux->next 		= NULL;

		if(this->fuzzySetOutputs == NULL){
			this->fuzzySetOutputs = aux;
			this->fuzzySetOutputsCursor	= aux;
		}else{
			this->fuzzySetOutputsCursor->next = aux;
			this->fuzzySetOutputsCursor = aux;
		}
		return true;
	}

	bool FuzzyRuleConsequent::evaluate(float power){
		fuzzySetOutputArray *aux;
		aux = this->fuzzySetOutputs;
		while(aux != NULL){
			aux->fuzzySet->setPertinence(power);
			aux = aux->next;
		}
		return true;
	}

	// M�TODOS PRIVADOS
	void FuzzyRuleConsequent::cleanFuzzySets(fuzzySetOutputArray* aux){
		if(aux != NULL){
			// Esvaziando a mem�ria alocada
			this->cleanFuzzySets(aux->next);
			free(aux);
		}
	}

	// CONSTRUTORES
	FuzzyRuleAntecedent::FuzzyRuleAntecedent(){
		this->op = 0;
		this->mode = 0;
		this->fuzzySet1 = NULL;
		this->fuzzySet2 = NULL;
		this->fuzzyRuleAntecedent1 = NULL;
		this->fuzzyRuleAntecedent2 = NULL;
	}

	// M�TODOS P�BLICOS
	bool FuzzyRuleAntecedent::joinSingle(FuzzySet* fuzzySet){
		if(fuzzySet){
			this->mode = MODE_FS;
			this->fuzzySet1 = fuzzySet;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithAND(FuzzySet* fuzzySet1, FuzzySet* fuzzySet2){
		if(fuzzySet1 != NULL and fuzzySet2 != NULL){
			this->op = OP_AND;
			this->mode = MODE_FS_FS;
			this->fuzzySet1 = fuzzySet1;
			this->fuzzySet2 = fuzzySet2;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithOR(FuzzySet* fuzzySet1, FuzzySet* fuzzySet2){
		if(fuzzySet1 != NULL and fuzzySet2 != NULL){
			this->op = OP_OR;
			this->mode = MODE_FS_FS;
			this->fuzzySet1 = fuzzySet1;
			this->fuzzySet2 = fuzzySet2;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithAND(FuzzySet* fuzzySet, FuzzyRuleAntecedent* fuzzyRuleAntecedent){
		if(fuzzySet != NULL and fuzzyRuleAntecedent != NULL){
			this->op = OP_AND;
			this->mode = MODE_FS_FRA;
			this->fuzzySet1 = fuzzySet;
			this->fuzzyRuleAntecedent1 = fuzzyRuleAntecedent;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithAND(FuzzyRuleAntecedent* fuzzyRuleAntecedent, FuzzySet* fuzzySet){
		if(fuzzySet != NULL and fuzzyRuleAntecedent != NULL){
			this->op = OP_AND;
			this->mode = MODE_FS_FRA;
			this->fuzzySet1 = fuzzySet;
			this->fuzzyRuleAntecedent1 = fuzzyRuleAntecedent;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithOR(FuzzySet* fuzzySet, FuzzyRuleAntecedent* fuzzyRuleAntecedent){
		if(fuzzySet != NULL and fuzzyRuleAntecedent != NULL){
			this->op = OP_OR;
			this->mode = MODE_FS_FRA;
			this->fuzzySet1 = fuzzySet;
			this->fuzzyRuleAntecedent1 = fuzzyRuleAntecedent;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithOR(FuzzyRuleAntecedent* fuzzyRuleAntecedent, FuzzySet* fuzzySet){
		if(fuzzySet != NULL and fuzzyRuleAntecedent != NULL){
			this->op = OP_OR;
			this->mode = MODE_FS_FRA;
			this->fuzzySet1 = fuzzySet;
			this->fuzzyRuleAntecedent1 = fuzzyRuleAntecedent;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithAND(FuzzyRuleAntecedent* fuzzyRuleAntecedent1, FuzzyRuleAntecedent* fuzzyRuleAntecedent2){
		if(fuzzyRuleAntecedent1 != NULL and fuzzyRuleAntecedent2 != NULL){
			this->op = OP_AND;
			this->mode = MODE_FRA_FRA;
			this->fuzzyRuleAntecedent1 = fuzzyRuleAntecedent1;
			this->fuzzyRuleAntecedent2 = fuzzyRuleAntecedent2;
			return true;
		}
		return false;
	}

	bool FuzzyRuleAntecedent::joinWithOR(FuzzyRuleAntecedent* fuzzyRuleAntecedent1, FuzzyRuleAntecedent* fuzzyRuleAntecedent2){
		if(fuzzyRuleAntecedent1 != NULL and fuzzyRuleAntecedent2 != NULL){
			this->op = OP_OR;
			this->mode = MODE_FRA_FRA;
			this->fuzzyRuleAntecedent1 = fuzzyRuleAntecedent1;
			this->fuzzyRuleAntecedent2 = fuzzyRuleAntecedent2;
			return true;
		}
		return false;
	}

	float FuzzyRuleAntecedent::evaluate(){
		switch(this->mode){
			case MODE_FS:
				return this->fuzzySet1->getPertinence();
				break;
			case MODE_FS_FS:
				switch(this->op){
					case OP_AND:
						if(this->fuzzySet1->getPertinence() > 0.0 and this->fuzzySet2->getPertinence() > 0.0){
							if(this->fuzzySet1->getPertinence() < this->fuzzySet2->getPertinence()){
								return this->fuzzySet1->getPertinence();
							}else{
								return this->fuzzySet2->getPertinence();
							}
						}else{
							return 0.0;
						}
						break;
					case OP_OR:
						if(this->fuzzySet1->getPertinence() > 0.0 or this->fuzzySet2->getPertinence() > 0.0){
							if(this->fuzzySet1->getPertinence() > this->fuzzySet2->getPertinence()){
								return this->fuzzySet1->getPertinence();
							}else{
								return this->fuzzySet2->getPertinence();
							}
						}else{
							return 0.0;
						}
						break;
					default:
						return 0.0;
				}
				break;
			case MODE_FS_FRA:
				switch(this->op){
					case OP_AND:
						if(this->fuzzySet1->getPertinence() > 0.0 and fuzzyRuleAntecedent1->evaluate() > 0.0){
							if(this->fuzzySet1->getPertinence() < fuzzyRuleAntecedent1->evaluate()){
								return this->fuzzySet1->getPertinence();
							}else{
								return fuzzyRuleAntecedent1->evaluate();
							}
						}else{
							return 0.0;
						}
						break;
					case OP_OR:
						if(this->fuzzySet1->getPertinence() > 0.0 or fuzzyRuleAntecedent1->evaluate() > 0.0){
							if(this->fuzzySet1->getPertinence() > fuzzyRuleAntecedent1->evaluate()){
								return this->fuzzySet1->getPertinence();
							}else{
								return fuzzyRuleAntecedent1->evaluate();
							}
						}else{
							return 0.0;
						}
						break;
					default:
						return 0.0;
				}
				break;
			case MODE_FRA_FRA:
				switch(this->op){
					case OP_AND:
						if(fuzzyRuleAntecedent1->evaluate() > 0.0 and fuzzyRuleAntecedent2->evaluate() > 0.0){
							if(fuzzyRuleAntecedent1->evaluate() < fuzzyRuleAntecedent2->evaluate()){
								return fuzzyRuleAntecedent1->evaluate();
							}else{
								return fuzzyRuleAntecedent2->evaluate();
							}
						}else{
							return 0.0;
						}
						break;
					case OP_OR:
						if(fuzzyRuleAntecedent1->evaluate() > 0.0 or fuzzyRuleAntecedent2->evaluate() > 0.0){
							if(fuzzyRuleAntecedent1->evaluate() > fuzzyRuleAntecedent2->evaluate()){
								return fuzzyRuleAntecedent1->evaluate();
							}else{
								return fuzzyRuleAntecedent2->evaluate();
							}
						}else{
							return 0.0;
						}
						break;
					default:
						return 0.0;
				}
				break;
			default:
				return 0.0;
		}
		return 0.0;
	}

	FuzzyRule::FuzzyRule(){
	}

	FuzzyRule::FuzzyRule(int index, FuzzyRuleAntecedent* fuzzyRuleAntecedent, FuzzyRuleConsequent* fuzzyRuleConsequent){
		this->index = index;
		this->fuzzyRuleAntecedent = fuzzyRuleAntecedent;
		this->fuzzyRuleConsequent = fuzzyRuleConsequent;
		this->fired = false;
	}

	int FuzzyRule::getIndex(){
		return this->index;
	}

	bool FuzzyRule::evaluateExpression(){
		if (this->fuzzyRuleAntecedent != NULL){
			float powerOfAntecedent = this->fuzzyRuleAntecedent->evaluate();

			(powerOfAntecedent > 0.0) ?	(this->fired = true) : (this->fired = false);

			this->fuzzyRuleConsequent->evaluate(powerOfAntecedent);
		}
		return this->fired;
	}

	bool FuzzyRule::isFired(){
		return this->fired;
	}

	// CONSTRUTORES
	FuzzyOutput::FuzzyOutput() : FuzzyIO(){
	}

	FuzzyOutput::FuzzyOutput(int index) : FuzzyIO(index){
	}

	// DESTRUTOR
	FuzzyOutput::~FuzzyOutput(){
	}

	// M�TODOS P�BLICOS
	bool FuzzyOutput::truncate(){
		// esvaziando a composi��o
		this->fuzzyComposition.empty();

		fuzzySetArray *aux;
		aux = this->fuzzySets;
		while(aux != NULL){
			if(aux->fuzzySet->getPertinence() > 0.0){
				if(this->fuzzyComposition.checkPoint(aux->fuzzySet->getPointA(), 0.0) == false){
					this->fuzzyComposition.addPoint(aux->fuzzySet->getPointA(), 0.0);
				}

				if(this->fuzzyComposition.checkPoint(aux->fuzzySet->getPointB(), aux->fuzzySet->getPertinence()) == false){
					this->fuzzyComposition.addPoint(aux->fuzzySet->getPointB(), aux->fuzzySet->getPertinence());
				}

				if(this->fuzzyComposition.checkPoint(aux->fuzzySet->getPointC(), aux->fuzzySet->getPertinence()) == false){
					this->fuzzyComposition.addPoint(aux->fuzzySet->getPointC(), aux->fuzzySet->getPertinence());
				}

				if(this->fuzzyComposition.checkPoint(aux->fuzzySet->getPointD(), 0.0) == false){
					this->fuzzyComposition.addPoint(aux->fuzzySet->getPointD(), 0.0);
				}
			}
			aux = aux->next;
		}

		this->fuzzyComposition.build();

		return true;
	}

	float FuzzyOutput::getCrispOutput(){
		return this->fuzzyComposition.avaliate();
	}

	// Um simples Bubble Sort
	bool FuzzyOutput::order(){
		fuzzySetArray *aux1;
		fuzzySetArray *aux2;

		aux1 = this->fuzzySets;
		aux2 = this->fuzzySets;

		while(aux1 != NULL){
			while(aux2 != NULL){
				if(aux2->next != NULL){
					if(aux2->fuzzySet->getPointA() > aux2->next->fuzzySet->getPointA()){
						this->swap(aux2, aux2->next);
					}
				}
				aux2 = aux2->next;
			}
			aux2 = this->fuzzySets;
			aux1 = aux1->next;
		}
		return true;
	}

	// M�TODOS PRIVADOS
	bool FuzzyOutput::swap(fuzzySetArray* fuzzySetA, fuzzySetArray* fuzzySetB){
		FuzzySet* aux;

		aux = fuzzySetA->fuzzySet;
		fuzzySetA->fuzzySet = fuzzySetB->fuzzySet;
		fuzzySetB->fuzzySet = aux;

		return true;
	}

	// CONSTRUTORES
	FuzzyIO::FuzzyIO(){
	}

	FuzzyIO::FuzzyIO(int index){
		this->index = index;
		// Iniciando os ponteiros como nulo
		this->fuzzySets			= NULL;
		this->fuzzySetsCursor	= NULL;
	}

	// DESTRUTOR
	FuzzyIO::~FuzzyIO(){
		this->cleanFuzzySets(this->fuzzySets);
	}

	// M�TODOS P�BLICOS
	int FuzzyIO::getIndex(){
		return this->index;
	}

	void FuzzyIO::setCrispInput(float crispInput){
		this->crispInput = crispInput;
	}

	float FuzzyIO::getCrispInput(){
		return this->crispInput;
	}

	bool FuzzyIO::addFuzzySet(FuzzySet* fuzzySet){
		fuzzySetArray *aux;
		// Alocando espa�o na mem�ria
		if((aux = (fuzzySetArray *) malloc(sizeof(fuzzySetArray))) == NULL){
			return false;
		}
		aux->fuzzySet 	= fuzzySet;
		aux->next 		= NULL;

		if(this->fuzzySets == NULL){
			this->fuzzySets = aux;
			this->fuzzySetsCursor	= aux;
		}else{
			this->fuzzySetsCursor->next = aux;
			this->fuzzySetsCursor = aux;
		}
		return true;
	}

	void FuzzyIO::resetFuzzySets(){
		fuzzySetArray* fuzzySetsAux;
	    fuzzySetsAux = this->fuzzySets;
	    // Calculando as pertin�ncias de totos os FuzzyInputs
	    while(fuzzySetsAux != NULL){
	        fuzzySetsAux->fuzzySet->reset();
	        fuzzySetsAux = fuzzySetsAux->next;
	    }
	}

	// M�TODOS PROTEGIDOS
	void FuzzyIO::cleanFuzzySets(fuzzySetArray *aux){
		if(aux != NULL){
			// Esvaziando a mem�ria alocada
			this->cleanFuzzySets(aux->next);
			free(aux);
		}
	}

	// CONSTRUTORES
	FuzzyInput::FuzzyInput() : FuzzyIO(){
	}

	FuzzyInput::FuzzyInput(int index) : FuzzyIO(index){
	}

	// DESTRUTOR
	FuzzyInput::~FuzzyInput(){
	}

	// M�TODOS P�BLICOS
	bool FuzzyInput::calculateFuzzySetPertinences(){
		fuzzySetArray *aux;
		aux = this->fuzzySets;

		while(aux != NULL){
			if (aux->fuzzySet != NULL){
				aux->fuzzySet->calculatePertinence(this->crispInput);
			}
			aux = aux->next;
		}

		return true;
	}

	// CONSTRUTORES
	FuzzyComposition::FuzzyComposition(){
		this->pointsCursor 	= NULL;
		this->points 		= NULL;
	}

	// DESTRUTOR
	FuzzyComposition::~FuzzyComposition(){
		this->cleanPoints(this->pointsCursor);
		this->cleanPoints(this->points);
	}

	bool FuzzyComposition::addPoint(float point, float pertinence){
		pointsArray* aux;
		// Alocando espa�o na mem�ria
		if((aux = (pointsArray *) malloc(sizeof(pointsArray))) == NULL){
			return false;
		}
		aux->previous = NULL;
		aux->point = point;
		aux->pertinence = pertinence;
		aux->next = NULL;

		if(this->points == NULL){
			this->points = aux;
			this->pointsCursor  = aux;
		}else{
			aux->previous = this->pointsCursor;
			this->pointsCursor = aux;
			aux->previous->next = this->pointsCursor;
		}
		return true;
	}

	bool FuzzyComposition::checkPoint(float point, float pertinence){
		pointsArray *aux;
		aux = this->pointsCursor;
		while(aux != NULL){
			if(aux->point == point && aux->pertinence == pertinence){
				return true;
			}
			aux = aux->previous;
		}
		return false;
	}

	bool FuzzyComposition::build(){
		pointsArray *aux;

		aux = this->points;
		while(aux != NULL){
			pointsArray *temp = aux;
			while(temp->previous != NULL){
				if(temp->point < temp->previous->point){
					break;
				}
				temp = temp->previous;
			}
			pointsArray* zPoint;
			if(temp != NULL){
				zPoint = temp;
				while(temp->previous != NULL){
					bool result = false;
					if(temp->previous->previous != NULL){
						result = rebuild(zPoint, zPoint->next, temp->previous, temp->previous->previous);
					}
					if(result == true){
						aux = this->points;
						break;
					}
					temp = temp->previous;
				}
			}
			aux = aux->next;
		}
		return true;
	}

	float FuzzyComposition::avaliate(){
		pointsArray *aux;
		float numerator 	= 0.0;
		float denominator 	= 0.0;

		aux = this->points;
		while(aux != NULL){
			if(aux->next != NULL){
				float area = 0.0;
				float middle = 0.0;
				if(aux->point == aux->next->point){
					// Se Singleton
					area 	= aux->pertinence;
					middle 	= aux->point;
				}else if(aux->pertinence == 0.0 || aux->next->pertinence == 0.0){
					// Se triangulo
					float pertinence;
					if(aux->pertinence > 0.0){
						pertinence = aux->pertinence;
					}else{
						pertinence = aux->next->pertinence;
					}
					area 	= ((aux->next->point - aux->point) * pertinence) / 2.0;
					middle 	= ((aux->next->point - aux->point) / 2.0) + aux->point;
				}else if((aux->pertinence > 0.0 && aux->next->pertinence > 0.0) && (aux->pertinence == aux->next->pertinence)){
					// Se quadrado
					area 	= (aux->next->point - aux->point) * aux->pertinence;
					middle 	= ((aux->next->point - aux->point) / 2.0) + aux->point;
				}else if((aux->pertinence > 0.0 && aux->next->pertinence > 0.0) && (aux->pertinence != aux->next->pertinence)){
					// Se trapezio
					area 	= ((aux->pertinence + aux->next->pertinence) / 2.0) * (aux->next->point - aux->point);
					middle 	= ((aux->next->point - aux->point) / 2.0) + aux->point;
				}
				numerator 	+= middle * area;
				denominator += area;
			}
			aux = aux->next;
		}

		if(denominator == 0.0){
			return 0.0;
		}else{
			return numerator / denominator;
		}
	}

	bool FuzzyComposition::empty(){
		// limpando a mem�ria
		this->cleanPoints(this->points);
		// resetando os ponteiros
		this->points = NULL;
		this->pointsCursor = NULL;
		return true;
	}

	// M�TODOS PRIVADOS
	void FuzzyComposition::cleanPoints(pointsArray* aux){
		if(aux != NULL){
			// Esvaziando a mem�ria alocada
			this->cleanPoints(aux->next);
			free(aux);
		}
	}

	bool FuzzyComposition::rebuild(pointsArray* aSegmentBegin, pointsArray* aSegmentEnd, pointsArray* bSegmentBegin, pointsArray* bSegmentEnd){
		float x1 = aSegmentBegin->point;
		float y1 = aSegmentBegin->pertinence;
		float x2 = aSegmentEnd->point;
		float y2 = aSegmentEnd->pertinence;
		float x3 = bSegmentBegin->point;
		float y3 = bSegmentBegin->pertinence;
		float x4 = bSegmentEnd->point;
		float y4 = bSegmentEnd->pertinence;
		float point, pertinence;
		float denom, numera, numerb;
		float mua, mub;

		denom  = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
		numera = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
		numerb = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);

		if(denom < 0.0){
			denom *= -1.0;
		}
		if(numera < 0.0){
			numera *= -1.0;
		}
		if(numerb < 0.0){
			numerb *= -1.0;
		}

		// Se os seguimentos forem paralelos, retornar falso
		if(denom < EPS){
			return false;
		}

		// Verificar se h� interse��o ao longo do seguimento
		mua = numera / denom;
		mub = numerb / denom;
		if(mua < 0.0 || mua > 1.0 || mub < 0.0 || mub > 1.0){
			return false;
		}else{
			// Calculando o ponto e a pertinencia do novo elemento
			point 		= x1 + mua * (x2 - x1);
			pertinence 	= y1 + mua * (y2 - y1);

			// Adicionando um novo ponto
			pointsArray* aux;
			// Alocando espa�o na mem�ria
			if((aux = (pointsArray *) malloc(sizeof(pointsArray))) == NULL){
				return false;
			}

			aux->previous = bSegmentEnd;
			aux->point = point;
			aux->pertinence = pertinence;
			aux->next = aSegmentEnd;

			bSegmentEnd->next = aux;
			aSegmentEnd->previous = aux;

			float stopPoint = bSegmentBegin->point;
			float stopPertinence = bSegmentBegin->pertinence;

			pointsArray* temp = aSegmentBegin;
			pointsArray* excl;

			do{
				float pointToCompare = temp->point;
				float pertinenceToCompare = temp->pertinence;

				excl = temp->previous;

				this->rmvPoint(temp);

				temp = excl;

				if(stopPoint == pointToCompare && stopPertinence == pertinenceToCompare){
					break;
				}
			}while(temp != NULL);

			return true;
		}
	}

	bool FuzzyComposition::rmvPoint(pointsArray* point){
		if(point != NULL){
			free(point);
		}
		return true;
	}

	// CONSTRUTORES
	Fuzzy::Fuzzy(){
	    // Iniciando os ponteiros como nulo
	    // FuzzyInput
	    this->fuzzyInputs       = NULL;
	    this->fuzzyInputsCursor = NULL;
	    // FuzzyOutput
	    this->fuzzyOutputs          = NULL;
	    this->fuzzyOutputsCursor    = NULL;
	    // FuzzyRule
	    this->fuzzyRules        = NULL;
	    this->fuzzyRulesCursor  = NULL;
	}

	// DESTRUTOR
	Fuzzy::~Fuzzy(){
	    this->cleanFuzzyInputs(this->fuzzyInputs);
	    this->cleanFuzzyOutputs(this->fuzzyOutputs);
	    this->cleanFuzzyRules(this->fuzzyRules);
	}

	// M�TODOS P�BLICOS
	bool Fuzzy::addFuzzyInput(FuzzyInput* fuzzyInput){
	    fuzzyInputArray* aux;

	    // Alocando espa�o na mem�ria
	    if((aux = (fuzzyInputArray *) malloc(sizeof(fuzzyInputArray))) == NULL){
	        return false;
	    }

	    aux->fuzzyInput = fuzzyInput;
	    aux->next = NULL;

	    if(this->fuzzyInputs == NULL){
	        this->fuzzyInputs = aux;
	        this->fuzzyInputsCursor  = aux;
	    }else{
	        this->fuzzyInputsCursor->next = aux;
	        this->fuzzyInputsCursor = aux;
	    }

	    return true;
	}

	bool Fuzzy::addFuzzyOutput(FuzzyOutput* fuzzyOutput){
	    fuzzyOutputArray *aux;
	    // Alocando espa�o na mem�ria
	    if((aux = (fuzzyOutputArray *) malloc(sizeof(fuzzyOutputArray))) == NULL){
	        return false;
	    }
	    aux->fuzzyOutput = fuzzyOutput;
	    aux->next = NULL;

	    // Ordenando o fuzzyOutput
	    fuzzyOutput->order();

	    if(this->fuzzyOutputs == NULL){
	        this->fuzzyOutputs = aux;
	        this->fuzzyOutputsCursor  = aux;
	    }else{
	        this->fuzzyOutputsCursor->next = aux;
	        this->fuzzyOutputsCursor = aux;
	    }
	    return true;
	}

	bool Fuzzy::addFuzzyRule(FuzzyRule* fuzzyRule){
	    fuzzyRuleArray* aux;
	    // Alocando espa�o na mem�ria
	    if((aux = (fuzzyRuleArray *) malloc(sizeof(fuzzyRuleArray))) == NULL){
	        return false;
	    }
	    aux->fuzzyRule = fuzzyRule;
	    aux->next = NULL;

	    if(this->fuzzyRules == NULL){
	        this->fuzzyRules = aux;
	        this->fuzzyRulesCursor  = aux;
	    }else{
	        this->fuzzyRulesCursor->next = aux;
	        this->fuzzyRulesCursor = aux;
	    }
	    return true;
	}

	bool Fuzzy::setInput(int fuzzyInputIndex, float crispValue){
	    fuzzyInputArray *aux;
	    aux = this->fuzzyInputs;

	    while(aux != NULL){
	        if(aux->fuzzyInput->getIndex() == fuzzyInputIndex){
	            aux->fuzzyInput->setCrispInput(crispValue);
	            return true;
	        }
	        aux = aux->next;
	    }
	    return false;
	}

	bool Fuzzy::fuzzify(){
	    fuzzyInputArray* fuzzyInputAux;

	    fuzzyOutputArray *fuzzyOutputAux;

	    fuzzyInputAux = this->fuzzyInputs;
	    while(fuzzyInputAux != NULL){
	        fuzzyInputAux->fuzzyInput->resetFuzzySets();
	        fuzzyInputAux = fuzzyInputAux->next;
	    }

	    fuzzyOutputAux = this->fuzzyOutputs;
	    while(fuzzyOutputAux != NULL){
	        fuzzyOutputAux->fuzzyOutput->resetFuzzySets();
	        fuzzyOutputAux = fuzzyOutputAux->next;
	    }

	    // Calculando a pertin�ncia de todos os FuzzyInputs
	    fuzzyInputAux = this->fuzzyInputs;
	    while(fuzzyInputAux != NULL){
	        fuzzyInputAux->fuzzyInput->calculateFuzzySetPertinences();
	        fuzzyInputAux = fuzzyInputAux->next;
	    }

	    // Avaliando quais regras foram disparadas
	    fuzzyRuleArray* fuzzyRuleAux;
	    fuzzyRuleAux = this->fuzzyRules;
	    // Calculando as pertin�ncias de totos os FuzzyInputs
	    while(fuzzyRuleAux != NULL){
	        fuzzyRuleAux->fuzzyRule->evaluateExpression();
	        fuzzyRuleAux = fuzzyRuleAux->next;
	    }

	    // Truncado os conjuntos de sa�da
	    fuzzyOutputAux = this->fuzzyOutputs;
	    while(fuzzyOutputAux != NULL){
	        fuzzyOutputAux->fuzzyOutput->truncate();
	        fuzzyOutputAux = fuzzyOutputAux->next;
	    }

	    return true;
	}

	bool Fuzzy::isFiredRule(int fuzzyRuleIndex){
	    fuzzyRuleArray *aux;
	    aux = this->fuzzyRules;
	    while(aux != NULL){
	        if(aux->fuzzyRule->getIndex() == fuzzyRuleIndex){
	            return aux->fuzzyRule->isFired();
	        }
	        aux = aux->next;
	    }
	    return false;
	}

	float Fuzzy::defuzzify(int fuzzyOutputIndex){
	    fuzzyOutputArray *aux;
	    aux = this->fuzzyOutputs;
	    while(aux != NULL){
	        if(aux->fuzzyOutput->getIndex() == fuzzyOutputIndex){
	            return aux->fuzzyOutput->getCrispOutput();
	        }
	        aux = aux->next;
	    }
	    return 0;
	}

	// M�TODOS PRIVADOS
	void Fuzzy::cleanFuzzyInputs(fuzzyInputArray* aux){
	    if(aux != NULL){
	        // Esvaziando a mem�ria alocada
	        this->cleanFuzzyInputs(aux->next);
	        free(aux);
	    }
	}

	void Fuzzy::cleanFuzzyOutputs(fuzzyOutputArray* aux){
	    if(aux != NULL){
	        // Esvaziando a mem�ria alocada
	        this->cleanFuzzyOutputs(aux->next);
	        free(aux);
	    }
	}

	void Fuzzy::cleanFuzzyRules(fuzzyRuleArray* aux){
	    if(aux != NULL){
	        // Esvaziando a mem�ria alocada
	        this->cleanFuzzyRules(aux->next);
	        free(aux);
	    }
	}

	//***********************************************************************************
	//******************* FIM DA IMPLEMENTA��O DA BIBLIOTECA ****************************
	//***********************************************************************************


	//***********************************************************************************
	//******************* IN�CIO DA IMPLEMENTA��O DA APLICA��O FUZZY*********************
	//***********************************************************************************


	// Exemplo de aplica��o
	// A MAIN TA NO LOOP
	//************************************************
	// Fim da biblioteca da logica fuzzy
	//************************************************
	Fuzzy* fuzzy;
	void setup()
	{
		pinMode(gas_sensor, INPUT);
		pinMode(porta_alarme, OUTPUT);
	  	pinMode(porta_motor, OUTPUT);
	  
	  Serial.begin(9600);
	 
	  	// Instanciando um objeto da biblioteca
		fuzzy = new Fuzzy();

		// Criando o FuzzyInput "fumaça/gas" (o nosso sensor pega de {487;842})
		FuzzyInput* vazamento_gas = new FuzzyInput(1);
		// Criando os FuzzySet que compoem o FuzzyInput distancia
		FuzzySet* small = new FuzzySet(487,546 ,546 , 605); // Distancia pequena [487,605]
		vazamento_gas->addFuzzySet(small); // Adicionando o FuzzySet small em gas
		FuzzySet* medium = new FuzzySet(588, 664,664, 723); // Distancia segura [605,723]
		vazamento_gas->addFuzzySet(medium); // Adicionando o FuzzySet medium em gas
		FuzzySet* big = new FuzzySet(704, 782,782 , 842); // Distancia grande [723,842]
		vazamento_gas->addFuzzySet(big); // Adicionando o FuzzySet big em gas

		fuzzy->addFuzzyInput(vazamento_gas); // Adicionando o FuzzyInput no objeto Fuzzy

		// Criando o FuzzyOutput motor
		FuzzyOutput* motor = new FuzzyOutput(1);
	  
		// Criando os FuzzySet que compoem o FuzzyOutput motor
		FuzzySet* slow = new FuzzySet(50, 80, 80, 120); // Velocidade lenta
		motor->addFuzzySet(slow); // Adicionando o FuzzySet slow em velocity
		
	  	FuzzySet* average = new FuzzySet(100, 150, 150, 190); // Velocidade normal
		motor->addFuzzySet(average); // Adicionando o FuzzySet average em velocity
		
	  	FuzzySet* fast = new FuzzySet(170, 220, 220, 255); // Velocidade alta
		motor->addFuzzySet(fast); // Adicionando o FuzzySet fast em velocity

		fuzzy->addFuzzyOutput(motor); // Adicionando o FuzzyOutput no objeto Fuzzy

		//Criando o Fuzzy Output alarme
		FuzzyOutput* alarme = new FuzzyOutput(2);

		//Criando os FuzzySet que compoem o FuzzyOutput alarme
		FuzzySet* baixo = new FuzzySet(600, 700, 700, 750); // Volume baixo
		alarme->addFuzzySet(baixo); // Adicionando o FuzzySet baixo em alarme
		
	  	FuzzySet* medio = new FuzzySet(720, 800, 800, 850); // Volume medio
		alarme->addFuzzySet(medio); // Adicionando o FuzzySet medio em alarme
		
	  	FuzzySet* alto = new FuzzySet(820, 900,900 , 1050); // Volume alto
		alarme->addFuzzySet(alto); // Adicionando o FuzzySet alto em alarme

		fuzzy->addFuzzyOutput(alarme); // Adicionando o FuzzyOutput no objeto Fuzzy

	  
	//COLOCAR AS REGRAS FUZZY AQUI :

	/////////REGRAS RELACIONANDO GAS COM MOTOR/////////////////
	//************************************
	// FuzzyRule "SE gas = SMALL entao MOTOR = slow"
		FuzzyRuleAntecedent* ifVazamento_Small = new FuzzyRuleAntecedent(); // Instanciando um Antecedente para a expresso
		ifVazamento_Small->joinSingle(small); // Adicionando o FuzzySet correspondente ao objeto Antecedente
	/// CONSEQUENCIAS
		FuzzyRuleConsequent* thenPoucoGas = new FuzzyRuleConsequent(); // Instancinado um Consequente para a expressao
		thenPoucoGas->addOutput(slow);// Adicionando o FuzzySet correspondente ao objeto Consequente
		thenPoucoGas->addOutput(baixo);// Adicionando o FuzzySet correspondente ao objeto Consequente

		FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifVazamento_Small, thenPoucoGas); // Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule01); // Adicionando o FuzzyRule ao objeto Fuzzy


	//************************************
	//FuzzyRule "SE GAS = MEDIO entao MOTOR = MEDIO"
		FuzzyRuleAntecedent* ifVazamento_Medium = new FuzzyRuleAntecedent(); // Instanciando um Antecedente para a expresso
		ifVazamento_Medium->joinSingle(medium); // Adicionando o FuzzySet correspondente ao objeto Antecedente
	/// CONSEQUENCIAS
		FuzzyRuleConsequent* thenMedioGas = new FuzzyRuleConsequent(); // Instancinado um Consequente para a expressao
		thenMedioGas->addOutput(average);// Adicionando o FuzzySet correspondente ao objeto Consequente
		thenMedioGas->addOutput(medio);

		FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifVazamento_Medium, thenMedioGas); // Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule02); // Adicionando o FuzzyRule ao objeto Fuzzy

	////////////////////////////
		// FuzzyRule "SE gas = BIG entao MOTOR = FAST"
		FuzzyRuleAntecedent* ifVazamento_Big = new FuzzyRuleAntecedent(); // Instanciando um Antecedente para a expresso
		ifVazamento_Big->joinSingle(big); // Adicionando o FuzzySet correspondente ao objeto Antecedente
	/// CONSEQUENCIAS
		FuzzyRuleConsequent* thenMuitoGas = new FuzzyRuleConsequent(); // Instancinado um Consequente para a expressao
		thenMuitoGas->addOutput(fast);// Adicionando o FuzzySet correspondente ao objeto Consequente
		thenMuitoGas->addOutput(alto);// Adicionando o FuzzySet correspondente ao objeto Consequente

		FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifVazamento_Big, thenMuitoGas); // Passando o Antecedente e o Consequente da expressao
		fuzzy->addFuzzyRule(fuzzyRule03); // Adicionando o FuzzyRule ao objeto Fuzzy

	}
	/////////REGRAS RELACIONANDO GAS COM ALARME//////////////



	/*float calcularDistancia(){
		float tempo, distancia;

		//const int gas_sensor = A0;
		//const int porta_alarme = 6;
		//const int porta_motor = 3;
	  
	  digitalWrite(TRIGGER, HIGH);
	  delayMicroseconds(50);
	  
	  digitalWrite(TRIGGER,LOW);
	  
	  tempo = pulseIn(ECHO,HIGH);
	  
	  distancia= tempo/2/29;
	  return distancia;
	}*/

	/*float funcao()
	{
		float intensidade;

		intensidade = analogRead(gas_sensor);

		return intensidade;
	}*/



	void loop()
	{	

			float gas;
			gas = analogRead(gas_sensor);


			fuzzy->setInput(1, gas);

			fuzzy->fuzzify();

			float output_motor = fuzzy->defuzzify(1);

			float output_alarme = fuzzy->defuzzify(2);
		
			Serial.println("Motor");
	        Serial.println(output_motor);
	        Serial.println("Alarme");
	        Serial.println(output_alarme); 
	        
	       	delay(10);

	       	analogWrite(porta_motor, output_motor);
	       	tone(6, output_alarme,500);

	       


	}
