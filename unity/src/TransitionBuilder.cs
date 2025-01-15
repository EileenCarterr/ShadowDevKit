using System;
using System.Collections.Generic;
using UnityEngine;

namespace ShadowDevKit.AI
{    
    public class TransitionBuilderBase<TState>
    {
        private TransitionBuilderBase() { }

        public static TransitionBlock Begin(ShadowAIEntity <TState> shadowAIEntity)
        {
            return new TransitionBlock( new List<Transition<TState>>(), shadowAIEntity );
        }

        public class TransitionBlock
        {
            private readonly List<Transition<TState>> _transitions;
            private Dictionary<TState, List<Transition<TState>>> transitionsMap;
			private ShadowAIEntity<TState> shadowAIEntity;
			
            public TransitionBlock( List<Transition<TState>> transitions, ShadowAIEntity<TState> shadowAIEntity )
            {
                _transitions = transitions;
				this.shadowAIEntity = shadowAIEntity;
            }

            public TransitionFromBuilder Transitions()
            {
                return new TransitionFromBuilder(this);
            }

            public TransitionBlock Finalize()
            {
				Validate();
                CreateTransitionsMap();
                return this;
            }

            public TState Build()
            {				
				if (transitionsMap.ContainsKey(shadowAIEntity.GetCurrentState()))
				{
					var transitions = transitionsMap[shadowAIEntity.GetCurrentState()];
					
                    foreach (var transition in transitions)
                    {
                        if (Array.TrueForAll(transition.Conditions, element => element() == true))
                        {
                            return transition.ToState;
                        }
                    }
				}

				Debug.LogWarning($"No valid transitions found for state: {shadowAIEntity.GetCurrentState()}");
                return default;
            }
			
			public void Validate()
			{
				if (_transitions.Count == 0)
					throw new InvalidOperationException("No transitions defined.");

				foreach (var transition in _transitions)
					if (transition.Conditions == null || transition.Conditions.Length == 0)
						throw new InvalidOperationException($"Transition from {transition.FromState} to {transition.ToState} is missing conditions.");
			}

            private void CreateTransitionsMap()
            {
                transitionsMap = new Dictionary<TState, List<Transition<TState>>>();

                foreach (var transition in _transitions)
                {
                    if (!transitionsMap.ContainsKey(transition.FromState))
                        transitionsMap[transition.FromState] = new List<Transition<TState>>();

                    transitionsMap[transition.FromState].Add(transition);
                }
            }

            public List<Transition<TState>> GetTransitions() => _transitions;

            public Dictionary<TState, List<Transition<TState>>> GetTransitionsMap() => transitionsMap;
        }

        public class TransitionFromBuilder
        {
            private readonly TransitionBlock _transitionBlock;

            public TransitionFromBuilder(TransitionBlock transitionBlock)
            {
                _transitionBlock = transitionBlock;
            }

            public TransitionToBuilder From(TState state)
            {
                return new TransitionToBuilder(state, _transitionBlock);
            }
        }

        public class TransitionToBuilder
        {
            private readonly TState _fromState;
            private readonly TransitionBlock _transitionBlock;

            public TransitionToBuilder(TState fromState, TransitionBlock transitionBlock)
            {
                _fromState = fromState;
                _transitionBlock = transitionBlock;
            }

            public TransitionConditionBuilder To(TState state)
            {
                return new TransitionConditionBuilder(_fromState, state, this, _transitionBlock);
            }
			
			public TransitionBlock End()
			{
				return _transitionBlock;
			}
			
            public TransitionBlock Default(TState state)
            {				
				_ = new TransitionConditionBuilder(_fromState, state, this, _transitionBlock)
						.Conditions( () => true );
						
				return _transitionBlock;
            }
        }

        public class TransitionConditionBuilder
        {
            private readonly TState _fromState;
            private readonly TState _toState;
            private readonly TransitionToBuilder _parentBuilder;
            private readonly TransitionBlock _transitionBlock;

            public TransitionConditionBuilder(TState fromState, TState toState, TransitionToBuilder parentBuilder, TransitionBlock transitionBlock)
            {
                _fromState = fromState;
                _toState = toState;
                _parentBuilder = parentBuilder;
                _transitionBlock = transitionBlock;
            }

            public TransitionToBuilder Conditions(params Func<bool>[] conditions)
            {
                _transitionBlock.GetTransitions().Add(new Transition<TState>(_fromState, _toState, conditions));
                return _parentBuilder; // Allows chaining of To statements
            }
        }
		
		public class Transition<SState>
		{
			public SState FromState { get; }
			public SState ToState { get; }
			public Func<bool>[] Conditions { get; }

			public Transition(SState fromState, SState toState, Func<bool>[] conditions)
			{
				FromState = fromState;
				ToState = toState;
				Conditions = conditions;
			}
		}
    }
}
