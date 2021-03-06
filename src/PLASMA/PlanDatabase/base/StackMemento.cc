#include "Token.hh"
#include "PlanDatabase.hh"
#include "StackMemento.hh"
#include "Constraint.hh"
#include "ConstraintType.hh"
#include <map>
#include <list>
#include <set>

namespace EUROPA{

  StackMemento::StackMemento(const TokenId activeTokenToStack, const TokenId activeToken)
      :m_activeTokenToStack(activeTokenToStack), m_activeToken(activeToken),
       m_stackConstraints() {

    // Iterate over all variables and impose equivalence constraints
    // between the corresponding variables
    const std::vector<ConstrainedVariableId>& stackVariables = activeTokenToStack->getVariables();
    const std::vector<ConstrainedVariableId>& activeVariables = activeToken->getVariables();

    check_error(stackVariables.size() == activeVariables.size());

    std::vector<ConstrainedVariableId> newScope;

    // Post equality constraints for all but the state variable
    for(unsigned int i=1; i<stackVariables.size(); i++) {
      newScope.clear();
      newScope.push_back(stackVariables[i]);
      newScope.push_back(activeVariables[i]);
      ConstraintId newConstraint = m_activeToken->getPlanDatabase()->getConstraintEngine()->createConstraint(
                                      LabelStr("eq"),
          							  newScope);
      check_error(newConstraint.isValid());
      m_stackConstraints.push_back(newConstraint);
    }
  }

  StackMemento::~StackMemento() {}

  void StackMemento::undo(bool){
    check_error(!m_stackConstraints.empty());
    // Delete all new constraints.
    cleanup(m_stackConstraints);
  }

  void StackMemento::handleAdditionOfInactiveConstraint(const ConstraintId){ }

  void StackMemento::handleRemovalOfInactiveConstraint(const ConstraintId){ }
}
