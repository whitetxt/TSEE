#include "../tsee.h"

/**
 * @brief Initialises the animation subsystem.
 */
bool TSEE_Animation_Init(TSEE *tsee) {
	tsee->animations = xmalloc(sizeof(*tsee->animations));
	tsee->animations->animations = TSEE_Array_Create();
	tsee->animations->global_active = true;
	tsee->animations->global_timer = 0;
	tsee->init->animation = true;
	return true;
}

bool TSEE_Animation_LoadFromSpriteSheet(TSEE *tsee, TSEE_Object *obj, char *fn, int numFrames, bool spriteFileIsHorizontal, double secsBetweenFrames) {
	SDL_Surface *rawSurf = IMG_Load(fn);
	if (!rawSurf) {
		TSEE_Error("Failed to load SpriteFile for Animation.\n");
		return false;
	}
	int width = rawSurf->w;
	int height = rawSurf->h;
	SDL_Texture *rawTex = SDL_CreateTextureFromSurface(tsee->window->renderer, rawSurf);
	TSEE_Array *frames = TSEE_Array_Create();
	SDL_Texture *originalRenderTarget = SDL_GetRenderTarget(tsee->window->renderer);
	if (spriteFileIsHorizontal) {
		if ((width % numFrames) != 0) {
			TSEE_Error("Attempted to load SpriteSheet with an invalid width and numFrames (width %s numFrames != 0).\n", "%");
			return false;
		}
		for (int i = 0; i < numFrames; i++) {
			SDL_Texture *tex = SDL_CreateTexture(tsee->window->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width / numFrames, height);
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
			SDL_Rect rect = {i * (width / numFrames), 0, width / numFrames, height};
			SDL_SetRenderTarget(tsee->window->renderer, tex);
			SDL_RenderCopy(tsee->window->renderer, rawTex, &rect, NULL);
			TSEE_Texture *tseeTex = xmalloc(sizeof(*tseeTex));
			tseeTex->texture = tex;
			tseeTex->rect = (SDL_Rect){0, 0, width / numFrames, height};
			tseeTex->path = strdup(fn);
			TSEE_Array_Append(frames, tseeTex);
		}
		SDL_SetRenderTarget(tsee->window->renderer, originalRenderTarget);
	} else {
		TSEE_Log("Vertical SpriteSheets are not implemented.\n");
	}
	TSEE_Object_Animation *anim = xmalloc(sizeof(*anim));
	anim->active = true;
	anim->current_frame = 0;
	anim->frames = frames;
	anim->obj = obj;
	anim->time_between_frames = secsBetweenFrames;
	anim->timer = 0;
	SDL_DestroyTexture(rawTex);
	SDL_FreeSurface(rawSurf);
	return true;
}

/**
 * @brief Runs a step for the animation system.
 *
 * Steps through objects, adds dt to their timers and applies new frames.
 * @return success status
 */
bool TSEE_Animation_RunStep(TSEE *tsee) {
	if (!tsee->init->animation) return false;
	if (!tsee->animations) return false;
	if (!tsee->animations->animations) return false;
	tsee->animations->global_timer += tsee->dt;
	for (size_t i = 0; i < tsee->animations->animations->size; i++) {
		TSEE_Object_Animation *anim =
			TSEE_Array_Get(tsee->animations->animations, i);
		if (!anim) {
			TSEE_Error("Failed to get animation.\n");
			continue;
		}
		if (!anim->active) continue;
		anim->timer += tsee->dt;
		while (anim->timer > anim->time_between_frames) {
			anim->timer -= anim->time_between_frames;
			anim->current_frame = (anim->current_frame + 1) % anim->frames->size;
		}
		TSEE_Object *obj = anim->obj;
		TSEE_Texture *orig = obj->texture;
		obj->texture = TSEE_Array_Get(anim->frames, anim->current_frame);
		if (!obj->texture) {
			TSEE_Error("Failed to get texture for animation.\n");
			obj->texture = orig;
			continue;
		}
	}
	return true;
}