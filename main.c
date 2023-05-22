#include "common.h"



#ifndef APP_H
#define APP_H

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Point mousePoint;
	TTF_Font *font;
	int x, y, mouseX, mouseY, windowWidth, windowHeight;
	bool up, down, left, right, leftClick, rightClick, menu;
} App_s;

typedef struct {
	SDL_Rect rect;
	SDL_Texture *texture;
	SDL_Surface *surface;
	char text[MAX_TEXT], prefix[MAX_TEXT];
	int textWidth, textHeight, textLength, centerX, centerY, arrLength;
	bool focused, colliding, leftClick, rightClick;
} Button_s;

typedef struct {
    int type;
    SDL_Rect rect;
} Tile_s;

#endif

void appLoop(App_s *app);

void initMenu(App_s *app);
void handleTextInput(Button_s *inputBox, SDL_Event event);
void checkCollision(App_s *app, Button_s *button);
void createButton(App_s *app, Button_s button[]);
void doInput(App_s *app, Button_s button[]);
void doKeyUp(App_s *app, SDL_KeyboardEvent *event);
void doKeyDown(App_s *app, Button_s button[], SDL_KeyboardEvent *event);
void renderPrepare(App_s *app);
void initWindow(App_s *app);
void cleanupWindow(App_s *app);

#ifdef _WIN32
#ifdef _WIN64
#define SDL_MAIN_HANDLED

int WinMain(int argc, char* argv[]) {
    SDL_SetMainReady();
    return main(argc, argv);
}
#endif
#endif

int main(int argc, char* argv[]) {
	App_s app;
	app.menu = true;
	app.windowWidth = SCREEN_WIDTH;
	app.windowHeight = SCREEN_HEIGHT;
	initWindow(&app);
	initMenu(&app);

	appLoop(&app);
	

	printf("Hello, world!");
	cleanupWindow(&app);
	
	return 0;
}

void appLoop(App_s *app) {
	SDL_Rect rect;
		rect.x = 100;    
		rect.y = 100;    
		rect.w = 200;    
		rect.h = 150;    
	Button_s button;


	while (1) {
		Uint32 currentFrameTime = SDL_GetTicks();
		
		renderPrepare(app);
		doInput(app, &button);

		SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(app->renderer, &rect);

		
		SDL_RenderPresent(app->renderer);
		SDL_RenderClear(app->renderer);


		Uint32 elapsedTime = SDL_GetTicks() - currentFrameTime;
		if (elapsedTime < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - elapsedTime);
		}
	}
}

void initMenu(App_s *app) {
	TTF_Init();
	Button_s button[BUTTON_AMT];
	button[0].rect.x = 100;
	button[0].rect.y = 100;
	button[0].rect.w = 300;
	button[0].rect.h = 100;
	char widthPrefix[] = "Level Width: ";
	char widthText[] = "300";
	strcpy_s(button[0].text, sizeof(widthText), widthText);
	strcpy_s(button[0].prefix, sizeof(widthPrefix), widthPrefix);

	button[1].rect.x = 800;
	button[1].rect.y = 100;
	button[1].rect.w = 300;
	button[1].rect.h = 100;
	char heightPrefix[] = "Level Height: ";
	char heightText[] = "30";
	strcpy_s(button[1].text, sizeof(heightText), heightText);
	strcpy_s(button[1].prefix, sizeof(heightPrefix), heightPrefix);

	button[2].rect.x = 450;
	button[2].rect.y = app->windowHeight / 2;
	button[2].rect.w = 300;
	button[2].rect.h = 100;
	button[2].focused = false;
	char startText[] = "START EDITOR";
	strcpy_s(button[2].text, sizeof(startText), startText);
	strcpy_s(button[2].prefix, 1, "");

	app->font = TTF_OpenFont("assets/font/slkscr.ttf", FONT_SIZE);
	if (app->font == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "UNABLE TO LOAD FONT %p\n", app->font);
 		exit(1);
	}
	else {
		while (app->menu) {
			Uint32 currentFrameTime = SDL_GetTicks();

			renderPrepare(app);

			createButton(app, button);
			checkCollision(app, &button[0]);
			checkCollision(app, &button[1]);
			checkCollision(app, &button[2]);


			SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(app->renderer, &button[0].rect);
			SDL_RenderCopy(app->renderer, button[0].texture, NULL, &button[0].rect);
			
			SDL_SetRenderDrawColor(app->renderer, 255, 0, 255, 255);
			SDL_RenderFillRect(app->renderer, &button[1].rect);
			SDL_RenderCopy(app->renderer, button[1].texture, NULL, &button[1].rect);
			
			SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
			SDL_RenderFillRect(app->renderer, &button[2].rect);
			SDL_RenderCopy(app->renderer, button[2].texture, NULL, &button[2].rect);
			
			doInput(app, &button[0]);
			doInput(app, &button[1]);
			doInput(app, &button[2]);
			if (button[0].colliding && button[0].leftClick) {

				button[0].focused = true;
			}
			else if (!button[0].colliding) {


				button[0].focused = false;
			}

			if (button[1].colliding && button[1].leftClick) {


				button[1].focused = true;
			}
			else if (!button[1].colliding) {


				button[1].focused = false;
			}
			if (button[2].colliding && button[2].leftClick) {
				app->menu = false;
			}

			SDL_RenderPresent(app->renderer);
			SDL_RenderClear(app->renderer);

			Uint32 elapsedTime = SDL_GetTicks() - currentFrameTime;
			if (elapsedTime < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - elapsedTime);
			}
			
			
		}
		SDL_DestroyTexture(button[0].texture);
		SDL_FreeSurface(button[0].surface);
		SDL_DestroyTexture(button[1].texture);
		SDL_FreeSurface(button[1].surface);
		SDL_DestroyTexture(button[2].texture);
		SDL_FreeSurface(button[2].surface);

	}
}

void handleTextInput(Button_s button[], SDL_Event event) {
	for (int i = 0; i < BUTTON_AMT; i++) {
		if (button[i].focused) {
			strcat_s(button[i].text, sizeof(button->text), event.text.text);
		}
	}
}

void initMap(Tile_s tileMap[MAP_HEIGHT][MAP_WIDTH]) {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			tileMap[y][x].type = 0;

			tileMap[y][x].rect.x = x * TILE_SIZE;
			tileMap[y][x].rect.y = y * TILE_SIZE;
			tileMap[y][x].rect.w = TILE_SIZE;
			tileMap[y][x].rect.h = TILE_SIZE;
		}
	}
}



void checkCollision(App_s *app, Button_s *button) {
	if (SDL_PointInRect(&app->mousePoint, &button->rect)) {
		button->colliding = true;
	}
	else {
		button->colliding = false;
	}
}

void createButton(App_s *app, Button_s button[]) {
	for (int i = 0; i < BUTTON_AMT; i++) {
		char prefixedText[2 * MAX_TEXT];
		strcpy(prefixedText, button[i].prefix);
		strcat(prefixedText, button[i].text);

		button[i].surface = TTF_RenderText_Solid(app->font, prefixedText, COLOR_BLACK);
		button[i].texture = SDL_CreateTextureFromSurface(app->renderer, button[i].surface);

		while (button[i].surface->w > button[i].rect.w) {
			button[i].surface->w /= 2;
			button[i].surface->h /= 2;
		}
		button[i].textWidth = button[i].surface->w;
		button[i].textHeight = button[i].surface->h;
		button[i].centerX = button[i].rect.x + (button[i].rect.w - button[i].textWidth) / 2;
		button[i].centerY = button[i].rect.y + (button[i].rect.h - button[i].textHeight) / 2;
	}
}

void renderPrepare(App_s *app) {
	SDL_SetRenderDrawColor(app->renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, COLOR_BLUE.a);
	SDL_RenderClear(app->renderer);
}

void initWindow(App_s *app) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	//SDL_SetRenderDrawColor(app->renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, COLOR_BLUE.a);
	app->window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_PRESENTVSYNC);
	
	renderPrepare(app);
}

void cleanupWindow(App_s *app) {
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	TTF_CloseFont(app->font);
	TTF_Quit();
	SDL_Quit();
}

void doInput(App_s *app, Button_s button[]) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				doKeyDown(app, button, &event.key);
				break;
			case SDL_KEYUP:
				doKeyUp(app, &event.key);
				break;
			case SDL_TEXTINPUT:
				handleTextInput(button, event);
			break;
			case SDL_MOUSEMOTION:
				app->mousePoint.x = event.motion.x;
				app->mousePoint.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_CaptureMouse(SDL_TRUE);
					app->leftClick = true;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].leftClick = true;
					}
					break;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					SDL_CaptureMouse(SDL_TRUE);
					app->rightClick = true;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].rightClick = true;
					}
					break;
				}
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					SDL_CaptureMouse(SDL_FALSE);
					app->leftClick = false;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].leftClick = false;
					}
					break;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					SDL_CaptureMouse(SDL_FALSE);
					app->rightClick = false;
					for (int i = 0; i < BUTTON_AMT; i++) {
						button[i].rightClick = false;
					}	
					break;
				}
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					app->windowWidth = event.window.data1;
					app->windowHeight = event.window.data2;
				}
				break;
			default:
				break;
		}
	}
}



void doKeyDown(App_s *app, Button_s button[], SDL_KeyboardEvent *event) {
	for (int i = 0; i < BUTTON_AMT; i++) {
		if (event->repeat == 0) {
			printf("BUTTON: leftClick: %d\trightClick: %d\tcolliding: %d\tfocused: %d\ttext: %d\n", button[i].leftClick, button[i].rightClick, button[i].colliding, button[i].focused, atoi(button[i].text));
			if (event->keysym.scancode == SDL_SCANCODE_BACKSPACE && button[i].focused) {
				printf("BUTTON: leftClick: %d\trightClick: %d\tcolliding: %d\tfocused: %d\ttext: %d\n", button[i].leftClick, button[i].rightClick, button[i].colliding, button[i].focused, atoi(button[i].text));
				int len = strlen(button[i].text);
				if (len > 0) {
					button[i].text[len - 1] = '\0';
					len--;
					if (len == 0) {
						strcpy(button[i].text, " ");
					}
				}
			}
		}
	}
}

void doKeyUp(App_s *app, SDL_KeyboardEvent *event) {
	if (event->repeat == 0) {
		// if (event->keysym.scancode == SDL_SCANCODE_A) {
		// 	app->left = false;
		// }
		// if (event->keysym.scancode == SDL_SCANCODE_D) {
		// 	app->right = false;
		// }
		// if (event->keysym.scancode == SDL_SCANCODE_LSHIFT && player->stamina > 0 && app->sprint ) {
		// 	player->speed /= SPRINT_SCALAR;
		// 	app->sprint = false;
		// }
	}
}
