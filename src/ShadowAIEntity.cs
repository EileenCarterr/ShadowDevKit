
namespace ShadowDevKit.AI
{
	public class ShadowAIEntity <T>
	{
		private T current_state;
		
		public T GetCurrentState() => current_state;
		public void SetCurrentState(T new_state) => current_state = new_state;
	}
}
