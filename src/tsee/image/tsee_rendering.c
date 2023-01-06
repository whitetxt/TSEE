#include "../tsee.h"

/**
 * @brief Initialises rendering for a TSEE object.
 *
 * @return success status
 */
bool TSEE_Rendering_Init(TSEE *tsee) {
	if (tsee->init->rendering) {
		return true;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		TSEE_Error("SDL could not initialize (%s)\n", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) ==
		0) {
		TSEE_Error("SDL_IMG could not initialize (%s)\n", IMG_GetError());
		return false;
	}

	tsee->window->window = SDL_CreateWindow(
		"No window title set: Call TSEE_Window_SetTitle",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tsee->window->width,
		tsee->window->height, SDL_WINDOW_SHOWN);
	if (tsee->window->window == NULL) {
		TSEE_Error("Window could not be created! SDL_Error: %s\n",
				   SDL_GetError());
		SDL_Quit();
		return false;
	}

	TSEE_Log("Width: %d, Height: %d\n", tsee->window->width,
			 tsee->window->height);

	tsee->window->renderer = SDL_CreateRenderer(
		tsee->window->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (tsee->window->renderer == NULL) {
		TSEE_Error("Renderer could not be created! SDL_Error: %s\n",
				   SDL_GetError());
		SDL_DestroyWindow(tsee->window->window);
		SDL_Quit();
		return false;
	}

	if (SDL_GetCurrentDisplayMode(0, &tsee->window->mode) != 0) {
		TSEE_Warn("Failed to get display mode (%s)\n", SDL_GetError());
		tsee->window->fps = 60;
	} else {
		tsee->window->fps = tsee->window->mode.refresh_rate;
		if (tsee->window->fps == 0) {
			TSEE_Warn("Unspecified refresh rate.\n");
			tsee->window->fps = 60;
		}
	}
	TSEE_Log("Framerate: %d\n", tsee->window->fps);

	tsee->init->rendering = true;
	return true;
}

/**
 * @brief Updates the window size for a TSEE object
 *
 */
void TSEE_Window_UpdateSize(TSEE *tsee) {
	SDL_SetWindowSize(tsee->window->window, tsee->window->width,
					  tsee->window->height);
}

/**
 * @brief Destroys a window and renderer for a TSEE_Window
 *
 * @param window Window to destroy
 */
void TSEE_Window_Destroy(TSEE_Window *window) {
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	if (window->title)
		xfree(window->title);
	xfree(window);
}

/**
 * @brief Sets the title of a window.
 *
 * @param title New title of the window.
 * @return success status
 */
bool TSEE_Window_SetTitle(TSEE *tsee, char *title) {
	if (!tsee->window->window) {
		TSEE_Error("Window is not initialized\n");
		return false;
	}

	SDL_SetWindowTitle(tsee->window->window, title);
	tsee->window->title = strdup(title);
	return true;
}

/**
 * @brief Render everything inside a TSEE object.
 *
 * @return success status
 */
bool TSEE_RenderAll(TSEE *tsee) {
	Uint64 start = SDL_GetPerformanceCounter();
	if (SDL_GetWindowFlags(tsee->window->window) & SDL_WINDOW_MINIMIZED) {
		// Don't render if minimised.
		// Limits max FPS to 30
		SDL_Delay(33);
		return true;
	}
	SDL_SetRenderDrawColor(tsee->window->renderer, 0, 0, 0, 255);
	SDL_RenderClear(tsee->window->renderer);

	// Render parallax objects
	for (size_t i = 0; i < tsee->world->parallax_objects->size; i++) {
		TSEE_Object *para = TSEE_Array_Get(tsee->world->parallax_objects, i);
		if (!TSEE_Parallax_Render(tsee, para)) {
			TSEE_Warn("Failed to render parallax object\n");
		}
	}

	// Render all objects
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *obj = TSEE_Array_Get(tsee->world->objects, i);
		if (!TSEE_Object_Render(tsee, obj)) {
			TSEE_Warn("Failed to render object\n");
		}
	}

	if (!TSEE_UI_Render(tsee)) {
		TSEE_Warn("Failed to render all of UI\n");
	}

	if (tsee->debug->active) {
		char text[128];
		SDL_Color colour = {255, 255, 255, SDL_ALPHA_OPAQUE};
		int height_off = 0;
		sprintf(text, "Event: %.3f ms", tsee->debug->event_time);
		TSEE_Object *eventText =
			TSEE_Text_Create(tsee, "_default", text, colour);
		if (eventText) {
			eventText->texture->rect.x = 0;
			eventText->texture->rect.y = height_off;
			height_off += eventText->texture->rect.h;
			SDL_SetRenderDrawColor(tsee->window->renderer, 100, 100, 100, 255);
			SDL_RenderFillRect(tsee->window->renderer,
							   &eventText->texture->rect);
			TSEE_Text_Render(tsee, eventText);
			TSEE_Text_Destroy(tsee, eventText, true);
		}
		sprintf(text, "Physics: %.3f ms", tsee->debug->physics_time);
		TSEE_Object *physicsText =
			TSEE_Text_Create(tsee, "_default", text, colour);
		if (physicsText) {
			physicsText->texture->rect.x = 0;
			physicsText->texture->rect.y = height_off;
			height_off += physicsText->texture->rect.h;
			SDL_RenderFillRect(tsee->window->renderer,
							   &physicsText->texture->rect);
			TSEE_Text_Render(tsee, physicsText);
			TSEE_Text_Destroy(tsee, physicsText, true);
		}
		sprintf(text, "Render: %.3f ms", tsee->debug->render_time);
		TSEE_Object *renderText =
			TSEE_Text_Create(tsee, "_default", text, colour);
		if (renderText) {
			renderText->texture->rect.x = 0;
			renderText->texture->rect.y = height_off;
			height_off += renderText->texture->rect.h;
			SDL_RenderFillRect(tsee->window->renderer,
							   &renderText->texture->rect);
			TSEE_Text_Render(tsee, renderText);
			TSEE_Text_Destroy(tsee, renderText, true);
		}
		sprintf(text, "Object Render: %.3f ms",
				tsee->debug->render_times.object_time);
		TSEE_Object *objRenderText =
			TSEE_Text_Create(tsee, "_default", text, colour);
		if (objRenderText) {
			objRenderText->texture->rect.x = 0;
			objRenderText->texture->rect.y = height_off;
			height_off += objRenderText->texture->rect.h;
			SDL_RenderFillRect(tsee->window->renderer,
							   &objRenderText->texture->rect);
			TSEE_Text_Render(tsee, objRenderText);
			TSEE_Text_Destroy(tsee, objRenderText, true);
		}
		sprintf(text, "Parallax Render: %.3f ms",
				tsee->debug->render_times.parallax_time);
		TSEE_Object *parallaxTimeText =
			TSEE_Text_Create(tsee, "_default", text, colour);
		if (parallaxTimeText) {
			parallaxTimeText->texture->rect.x = 0;
			parallaxTimeText->texture->rect.y = height_off;
			height_off += parallaxTimeText->texture->rect.h;
			SDL_RenderFillRect(tsee->window->renderer,
							   &parallaxTimeText->texture->rect);
			TSEE_Text_Render(tsee, parallaxTimeText);
			TSEE_Text_Destroy(tsee, parallaxTimeText, true);
		}
		sprintf(text, "Frame: %.3f ms", tsee->debug->frame_time);
		TSEE_Object *frameTimeText =
			TSEE_Text_Create(tsee, "_default", text, colour);
		if (frameTimeText) {
			frameTimeText->texture->rect.x = 0;
			frameTimeText->texture->rect.y = height_off;
			height_off += frameTimeText->texture->rect.h;
			SDL_RenderFillRect(tsee->window->renderer,
							   &frameTimeText->texture->rect);
			TSEE_Text_Render(tsee, frameTimeText);
			TSEE_Text_Destroy(tsee, frameTimeText, true);
		}
		sprintf(text, "Framerate: %.3f", 1000 / tsee->debug->frame_time);
		TSEE_Object *fpsText = TSEE_Text_Create(tsee, "_default", text, colour);
		if (fpsText) {
			fpsText->texture->rect.x = 0;
			fpsText->texture->rect.y = height_off;
			height_off += fpsText->texture->rect.h;
			SDL_RenderFillRect(tsee->window->renderer, &fpsText->texture->rect);
			TSEE_Text_Render(tsee, fpsText);
			TSEE_Text_Destroy(tsee, fpsText, true);
		}
	}

	SDL_RenderPresent(tsee->window->renderer);
	tsee->debug->framerate =
		1000 / (tsee->window->last_render - SDL_GetPerformanceCounter()) /
		(double)SDL_GetPerformanceFrequency();
	tsee->window->last_render = SDL_GetPerformanceCounter();
	tsee->debug->render_time = (tsee->window->last_render - start) * 1000 /
							   (double)SDL_GetPerformanceFrequency();
	tsee->debug->frame_time = tsee->debug->event_time +
							  tsee->debug->physics_time +
							  tsee->debug->render_time;
	tsee->debug->event_time = 0;
	tsee->debug->physics_time = 0;
	tsee->debug->render_times.object_time = 0;
	tsee->debug->render_times.parallax_time = 0;
	return true;
}

/**
 * @brief Checks if the TSEE is ready to render yet, according to the framerate
 *        set in the window object.
 *
 * @return true if ready, false if not.
 */
bool TSEE_Rendering_IsReady(TSEE *tsee) {
	double timeBetweenFrames = 1.0f / tsee->window->fps;
	double dt =
		(double)((SDL_GetPerformanceCounter() - tsee->window->last_render) /
				(double)SDL_GetPerformanceFrequency());
	return dt - timeBetweenFrames > -0.1f;
}