#include "STNTemporalAdvisor.hh"
#include "Token.hh"
#include "Object.hh"
#include "TokenVariable.hh"
#include "PlanDatabase.hh"
#include "TemporalNetworkDefs.hh"
#include "TemporalPropagator.hh"
#include "Debug.hh"
#include "Utils.hh"

namespace EUROPA {

  STNTemporalAdvisor::STNTemporalAdvisor(const TemporalPropagatorId propagator)
    : DefaultTemporalAdvisor(propagator->getConstraintEngine()), m_propagator(propagator) {}

  STNTemporalAdvisor::~STNTemporalAdvisor(){}

  bool STNTemporalAdvisor::canPrecede(const TokenId first, const TokenId second){    
    if (!DefaultTemporalAdvisor::canPrecede(first, second))
      return false;

    bool retval = m_propagator->canPrecede(first->end(), second->start());
    return (retval);
  }

  bool STNTemporalAdvisor::canPrecede(const TimeVarId first, const TimeVarId second) {
    if(!DefaultTemporalAdvisor::canPrecede(first, second))
      return false;
    return m_propagator->canPrecede(first, second);
  }

  bool STNTemporalAdvisor::canFitBetween(const TokenId token, const TokenId predecessor, const TokenId successor){
    if (!DefaultTemporalAdvisor::canFitBetween(token, predecessor, successor))
      return false;
    return m_propagator->canFitBetween(token->start(), token->end(), predecessor->end(), successor->start());
  }

  /**
   * @brief 2 tokens can be concurrent if the temporal distance between them can be 0
   */
  bool STNTemporalAdvisor::canBeConcurrent(const TokenId first, const TokenId second){
    debugMsg("STNTemporalAdvisor:canBeConcurrent", "first [" << first->start() << ", " << first->end() << "]");
    debugMsg("STNTemporalAdvisor:canBeConcurrent", "second[" << second->start() << ", " << second->end() << "]"); 

   return (m_propagator->canBeConcurrent(first->start(), second->start()) &&
	    m_propagator->canBeConcurrent(first->end(), second->end()));
  }

  /**
   * @brief Gets the temporal distance between two temporal variables. 
   * @param exact if set to true makes this distance calculation exact.
   */
  const IntervalIntDomain STNTemporalAdvisor::getTemporalDistanceDomain(const TimeVarId first, const TimeVarId second, const bool exact) {
    if( first->getExternalEntity().isNoId() 
	||
	second->getExternalEntity().isNoId() )
      {
	eint f_lb = static_cast<eint>(first->getLastDomain().getLowerBound());
	eint f_ub = static_cast<eint>(first->getLastDomain().getUpperBound());
	
	eint s_lb = static_cast<eint>(second->getLastDomain().getLowerBound());
	eint s_ub = static_cast<eint>(second->getLastDomain().getUpperBound());
	
	eint min_distance = MINUS_INFINITY;//-g_infiniteTime();

	// if( s_lb > -g_infiniteTime() && f_ub < g_infiniteTime() ) {
        if( s_lb > MINUS_INFINITY && f_ub < PLUS_INFINITY ) {
	    min_distance = std::max( min_distance, s_lb - f_ub );
	  }
	  
	eint max_distance = PLUS_INFINITY;//g_infiniteTime();
	
	// if( f_lb > -g_infiniteTime() && s_ub < g_infiniteTime() ) {
        if( f_lb > MINUS_INFINITY && s_ub < PLUS_INFINITY ) {
	  max_distance = std::min( max_distance, s_ub - f_lb );
        }

	return(IntervalIntDomain( min_distance, max_distance ));
      }

    return (m_propagator->getTemporalDistanceDomain(first, second, exact));
  }

  /**
   * @brief Gets the temporal distances from one to several other variables. 
   * More efficient to compute several simultaneously.  Always exact.
   */
  void STNTemporalAdvisor::getTemporalDistanceDomains(const ConstrainedVariableId first,
                                                      const std::vector<ConstrainedVariableId>&
                                                      seconds,
                                                      std::vector<IntervalIntDomain>& domains) {
    return (m_propagator->getTemporalDistanceDomains(first, seconds, domains));
  }

  /**
   * @brief Similar to getTemporalDistanceDomains, but propagates only far enough
   * so that the signs (but not values) of lbs/ubs are accurate.  Can be used to
   * to accurately and more quickly answer <= 0 and >= 0 questions for lb and ub.
   */
  void STNTemporalAdvisor::getTemporalDistanceSigns(const ConstrainedVariableId first,
                                                    const std::vector<ConstrainedVariableId>&
                                                    seconds,
                                                    std::vector<eint>& lbs,
                                                    std::vector<eint>& ubs) {
    std::vector<Time> tLbs, tUbs;
    m_propagator->getTemporalDistanceSigns(first, seconds, tLbs, tUbs);
    std::copy(tLbs.begin(), tLbs.end(), std::back_inserter(lbs));
    std::copy(tUbs.begin(), tUbs.end(), std::back_inserter(ubs));
  }



  unsigned int STNTemporalAdvisor::mostRecentRepropagation() const{
    return m_propagator->mostRecentRepropagation();
  }
}
