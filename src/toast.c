#include "include/main.h"

Toast *toasts = NULL;
size_t toastCount = 0;
SDL_Texture *toastBack = NULL;
int animTimer = 0;

// Creates a toast message and adds it to the list of toasts.
// @param text The text to display on the toast.
// @return true on success, false on failure.
bool createToast(char *text) {
	if (!toastBack) {
		toastBack = loadImage(renderer, "textures/toastBack.png");
	}
	Toast toast;
	toast.text = strdup(text);
	toast.textObj = createText(text, 0, 0, toastFont);
	toasts = realloc(toasts, ++toastCount * sizeof(*toasts));
	toasts[toastCount - 1] = toast;
	return true;
}

void renderToasts() {
	if (toastCount == 0) {
		return;
	}
	if (animTimer >= 0 && animTimer <= 10) {
		SDL_Rect toastBackRect = createRect(toastBack, width / 2, height - 50);
		toastBackRect.x -= toastBackRect.w / 2;
		if (toasts[0].textObj.rect.w > toastBackRect.w) {
			toastBackRect.w = toasts[0].textObj.rect.w + 20;
			toastBackRect.x = width / 2 - toastBackRect.w / 2;
		}
		SDL_SetTextureBlendMode(toastBack, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(toastBack, animTimer * 25);
		SDL_RenderCopy(renderer, toastBack, NULL, &toastBackRect);
		toasts[0].textObj.rect.x = (toastBackRect.x + toastBackRect.w / 2) - toasts[0].textObj.rect.w / 2;
		toasts[0].textObj.rect.y = (toastBackRect.y + toastBackRect.h / 2) - toasts[0].textObj.rect.h / 2;
		SDL_SetTextureBlendMode(toasts[0].textObj.tex, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(toasts[0].textObj.tex, animTimer * 12);
		renderText(toasts[0].textObj);
	} else if (animTimer > 10 && animTimer <= 110) {
		SDL_SetTextureBlendMode(toastBack, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(toastBack, 255);
		SDL_Rect toastBackRect = createRect(toastBack, width / 2, height - 50);
		toastBackRect.x -= toastBackRect.w / 2;
		if (toasts[0].textObj.rect.w > toastBackRect.w) {
			toastBackRect.w = toasts[0].textObj.rect.w + 20;
			toastBackRect.x = width / 2 - toastBackRect.w / 2;
		}
		SDL_RenderCopy(renderer, toastBack, NULL, &toastBackRect);
		toasts[0].textObj.rect.x = (toastBackRect.x + toastBackRect.w / 2) - toasts[0].textObj.rect.w / 2;
		toasts[0].textObj.rect.y = (toastBackRect.y + toastBackRect.h / 2) - toasts[0].textObj.rect.h / 2;
		SDL_SetTextureBlendMode(toasts[0].textObj.tex, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(toasts[0].textObj.tex, (animTimer / 20) * 255);
		renderText(toasts[0].textObj);
	} else if (animTimer > 110 && animTimer <= 120) {
		int tmp = abs(animTimer - 140);
		SDL_Rect toastBackRect = createRect(toastBack, width / 2, height - 50);
		toastBackRect.x -= toastBackRect.w / 2;
		if (toasts[0].textObj.rect.w > toastBackRect.w) {
			toastBackRect.w = toasts[0].textObj.rect.w + 20;
			toastBackRect.x = width / 2 - toastBackRect.w / 2;
		}
		SDL_SetTextureBlendMode(toastBack, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(toastBack, tmp * 25);
		SDL_RenderCopy(renderer, toastBack, NULL, &toastBackRect);
		toasts[0].textObj.rect.x = (toastBackRect.x + toastBackRect.w / 2) - toasts[0].textObj.rect.w / 2;
		toasts[0].textObj.rect.y = (toastBackRect.y + toastBackRect.h / 2) - toasts[0].textObj.rect.h / 2;
		SDL_SetTextureBlendMode(toasts[0].textObj.tex, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(toasts[0].textObj.tex, tmp * 25);
		renderText(toasts[0].textObj);
	} else {
		animTimer = -1;
		freeText(toasts[0].textObj);
		for (size_t i = 0; i < toastCount; i++) {
			toasts[i] = toasts[i + 1];
		}
		toasts = realloc(toasts, --toastCount * sizeof(*toasts));
	}
	animTimer++;
}