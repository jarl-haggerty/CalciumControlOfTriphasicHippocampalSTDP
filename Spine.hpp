
class Spine {
public:
  Spine();
  ~Spine();
  void simulate(const double time);
  void setPreSynapticSpike(const double time);
  void setPostSynapticSpike(const double time);
private:
  class Private;
  Private* priv;
};
