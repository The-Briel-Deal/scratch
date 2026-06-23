#include <SDL3/SDL.h>

#include <SDL3/SDL_error.h>
#include <print>

int main() {
  SDL_Haptic *haptic = NULL;

  // Open the device
  int count;
  SDL_HapticID *haptics = SDL_GetHaptics(&count);
    std::println("SDL_GetHaptics() found {:d} haptics devices.", count);

  if (haptics == NULL) {
    std::println("SDL_GetHaptics() returned null: {:s}", SDL_GetError());
    return 1;
  }

  if (haptics) {
    haptic = SDL_OpenHaptic(haptics[0]);
    SDL_free(haptics);
  }
  if (haptic == NULL) {
    std::println("SDL_OpenHaptic() returned null: {:s}", SDL_GetError());
    return 1;
  }

  // Initialize simple rumble
  if (!SDL_InitHapticRumble(haptic))
    return 1;

  // Play effect at 50% strength for 2 seconds
  if (!SDL_PlayHapticRumble(haptic, 0.5, 2000))
    return 1;
  SDL_Delay(2000);

  // Clean up
  SDL_CloseHaptic(haptic);
  return 0;
}
