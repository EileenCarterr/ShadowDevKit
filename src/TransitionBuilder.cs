using System;
using System.Collections.Generic;
using UnityEngine;

namespace ShadowDevKit.TransitionBuilder 
{
	public class TransitionBuilder<TState>
	{
		private TransitionBuilder() { }

		public static TransitionBlock Begin()
		{
			return new TransitionBlock(new List<Transition<TState>>());
		}

		public class TransitionBlock
		{
			private readonly List<Transition<TState>> _transitions;


			public TransitionBlock(List<Transition<TState>> transitions)
			{
				_transitions = transitions;
			}

			public TransitionFromBuilder Transitions()
			{
				return new TransitionFromBuilder(this);
			}

			public void Build()
			{
				Debug.Log("Building transitions:");
				foreach (var transition in _transitions)
					Debug.Log($"From: {transition.FromState}, To: {transition.ToState}, Conditions: {transition.Conditions.Length}");
			}
			
			public List<Transition<TState>> GetTransitions()
			{
				return _transitions;
			}
			
			// This method provides a dictionary for efficient lookups
            public Dictionary<TState, List<Transition<TState>>> GetTransitionsMap()
            {
                var transitionMap = new Dictionary<TState, List<Transition<TState>>>();

                foreach (var transition in _transitions)
                {
                    if (!transitionMap.ContainsKey(transition.FromState))
                        transitionMap[transition.FromState] = new List<Transition<TState>>();
					
                    transitionMap[transition.FromState].Add(transition);
                }

                return transitionMap;
            }
		}

		public class TransitionFromBuilder
		{
			private readonly TransitionBlock _transitionBlock;
			private TState _currentFromState;
			
			public TransitionBlock TransitionBlock {
				get { return _transitionBlock; }
			}
			
			
			public TransitionFromBuilder(TransitionBlock transitionBlock)
			{
				_transitionBlock = transitionBlock;
			}

			public TransitionFromBuilder From(TState state)
			{
				_currentFromState = state;
				return this;
			}

			public TransitionToBuilder To(TState state)
			{
				return new TransitionToBuilder(_currentFromState, state, this);
			}

			public TransitionBlock End()
			{
				return _transitionBlock;
			}
		}

		public class TransitionToBuilder
		{
			private readonly TState _fromState;
			private readonly TState _toState;
			private readonly TransitionFromBuilder _parentBuilder;


			public TransitionToBuilder(TState fromState, TState toState, TransitionFromBuilder parentBuilder)
			{
				_fromState = fromState;
				_toState = toState;
				_parentBuilder = parentBuilder;
			}

			public TransitionFromBuilder Conditions(params Func<bool>[] conditions)
			{
				_parentBuilder.TransitionBlock.GetTransitions().Add(new Transition<TState>(_fromState, _toState, conditions));
				return _parentBuilder;
			}
		}
	}

	public class Transition<TState>
	{
		public TState FromState { get; }
		public TState ToState { get; }
		public Func<bool>[] Conditions { get; }

		public Transition(TState fromState, TState toState, Func<bool>[] conditions)
		{
			FromState = fromState;
			ToState = toState;
			Conditions = conditions;
		}
	}
}
