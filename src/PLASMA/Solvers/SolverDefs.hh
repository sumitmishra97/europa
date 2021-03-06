#ifndef H_SolverDefs
#define H_SolverDefs

#include "PlanDatabaseDefs.hh"
#include "Entity.hh"

#include <vector>

namespace EUROPA {
  namespace SOLVERS {

    class Component;
    typedef Id<Component> ComponentId;

    class MatchingEngine;
    typedef Id<MatchingEngine> MatchingEngineId;

    class MatchingRule;
    typedef Id<MatchingRule> MatchingRuleId;

    class FlawFilter;
    typedef Id<FlawFilter> FlawFilterId;

    class FlawHandler;
    typedef Id<FlawHandler> FlawHandlerId;

    class FlawManager;
    typedef Id<FlawManager> FlawManagerId;

    typedef std::list<FlawManagerId> FlawManagers;

    class DecisionPoint;
    typedef Id<DecisionPoint> DecisionPointId;

    class Solver;
    typedef Id<Solver> SolverId;

    class Context;
    typedef Id<Context> ContextId;

    typedef std::vector<DecisionPointId> DecisionStack;

    typedef double Priority; /*!< Used to reference to the priority used in calculating heuristics. */

    /**
     * @brief Used to store guard entry data - var name and expected value to match on.
     */
    typedef std::pair<LabelStr, edouble> GuardEntry;

    static Priority & worstCasePriority() {
      static Priority sl_worstCasePriority(99999);
      return sl_worstCasePriority;
    }

    static Priority & bestCasePriority() {
      static Priority sl_bestCasePriority(0);
      return sl_bestCasePriority;
    }
  
    inline Priority getBestCasePriority() {
      return bestCasePriority();
    }

    inline Priority getWorstCasePriority() {
      return worstCasePriority();
    }

    /**
     * @brief Defines a configuration class
     */
    class SolverConfig{
    public:
      static void init();
      static void uninit();
      
      SolverConfig() 
      {
    	  static bool sl_registerComponents = false;
          if(sl_registerComponents == false){
	          init();
	          sl_registerComponents = true;
	      }
      }
    };

    namespace PlanWriter {
      class PartialPlanWriter;
    }
  }
}
#endif
