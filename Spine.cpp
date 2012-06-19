#include "Spine.hpp"

class Spine::Private {
public:
  double capacitance;
  double surfaceArea;
  double leakConductance;
  double leakReversalPotential;
  double ampaConductance;
  double ampaReversalPotential;
  double ampaProbability;
  double nmdaConductance;
  double nmdaReversalPotential;
  double nmdaProbability;
  double nmdaNormalization;
  double magnesium;
  double calcium;
  double lastPresynapticSpike;
  double lastPostSynapticSpike;
  double releaseProbabilityBase;
  double releaseProbabilityTimeConstant;
  double lastPresynapticSpike;
  double lastPostSynapticSpike;
  double potential;
  double membranePotential;
  double maxBackPropogationPotential;
  double fastBackPropogationContribution;
  double slowBackPropogationContribution;
  double currentTime;
  Private() {
    double magnesium = 1;
    nmdaNormalization = 1/(pow(nmdaTimeConstantFast/nmdaTimeConstantSlow, nmdaTimeConstantRise/nmdaTimeConstantSlow) -
			   pow(nmdaTimeConstantFast/nmdaTimeConstantSlow, nmdaTimeConstantRise/nmdaTimeConstantFast));
  }

  double backPropogationPotential(const double time) const {
    return maxBackPropogationPotential*(fastBackPropogationContribution*exp((lastPostSynapticSpike - time)/backPropogationTimeConstantFast) +
					slowBackPropogationContribution*exp((lastPostSynapticSpike - time)/backPropogationTimeConstantSlow))
  }

  double current(const double potential, const double time) const {
    return synapticCurrent(potential, time)/surfaceArea - leakCurrent(potential);
  }

  double leakCurrent(const double potential) const {
    return leakConductance*(potential - leakReversalPotential);
  }

  double synapticCurrent(const double potential, const double time) const {
    return ampaCurrent(potential, time) + nmdaCurrent(potential, time);
  }

  double ampaCurrent(const double potential, const double time) const {
    return -ampaConductance*ampaReleaseProbability(time)*(potential - ampaReversalPotential);
  }

  double ampaReleaseProbability(const double time) const {
    return releaseProbability(time)*exp((lastPresynapticSpike - time)/ampaTimeConstant);
  }

  double nmdaCurrent(const double potential, const double time) const {
    return -nmdaConductance*nmdaReleaseProbability(time)*nmdaMagnesiumBloackade(potential)*(potential - nmdaReversalPotential);
  }

  double nmdaReleaseProbability(const double time) const {
    return releaseProbability(time)*nmdaNormalization*(exp((lastPresynapticSpike - time)/nmdaTimeConstantFast) - 
						       exp((lastPresynapticSpike - time)/nmdaTimeConstantSlow));
  }

  double nmdaMagnesiumBloackade(const double potential) const {
    return 1/(1 + magnesium/3.57*exp(-potential/16.13));
  }

  double releaseProbability(const double time) const {
    return releaseProbabilityBase*(1 - exp((lastPresynapticSpike - time)/releaseProbabilityTimeConstant))
  }
};

Spine::Spine() : priv(new Private()) {}
Spine::~Spine() {
  delete priv;
}

void Spine::simulate(const double deltaTime) {
  double membranePotentialK1, membranePotentialK2, membranePotentialK3, membranePotentialK4;

  membranePotentialK1 = deltaTime*priv->current(priv->membranePotential, priv->currentTime)/priv->capacitance;
  membranePotentialK2 = deltaTime*priv->current(priv->membranePotential + membranePotentialK1/2, priv->currentTime + deltaTime/2)/priv->capacitance;
  membranePotentialK3 = deltaTime*priv->current(priv->membranePotential + membranePotentialK2/2, priv->currentTime + deltaTime/2)/priv->capacitance;
  membranePotentialK4 = deltaTime*priv->current(priv->membranePotential + membranePotentialK3, priv->currentTime + deltaTime)/priv->capacitance;

  priv->membranePotential += (membranePotentialK1 + membranePotentialK2 + membranePotentialK3 + membranePotentialK4)/6;
  priv->potential = priv->membranePotential + priv->backPropogationPotential(time);
}

void Spine::setPreSynapticSpike(const double time) {
  priv->lastPresynapticSpike = time;
}

void Spine::setPostSynapticSpike(const double time) {
  priv->lastPostSynapticSpike = time;
}
