#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Map.h"
j1Player::j1Player() : j1Module()
{
	active = false;
	name.create("player");
}
bool j1Player:: Awake (pugi::xml_node &node) 
{
	LOG("Init SDL player");
	
	String_docXml.create( node.child_value());
	
	return true;
}
void j1Player::Init()
{
	
	active = true;
}
bool j1Player::Start()
{
	bool ret = false;
	//Loading file player xml --------------------------------------------------------------
	pugi::xml_document	player_file;
	pugi::xml_parse_result result = player_file.load_file(String_docXml.GetString());

	ptexture = App->tex->Load("textures/skeleton.png");


	if (ptexture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	else {
		LOG("Loaded player texture succesfully");
	}

	if (result)
	{
		player_node = player_file.child("player");
		
		instantPos.x = player_node.child("player1").attribute("Start_pos_x").as_float();
		instantPos.y = player_node.child("player1").attribute("Start_pos_y").as_float();

		/*initialPos.x= player_node.child("player1").attribute("Start_pos_x").as_float();
		initialPos.y = player_node.child("player1").attribute("Start_pos_y").as_float();*/

		PlayerIdle = LoadAnimations("idle");
		PlayerWalk = LoadAnimations("walking");
		PlayerJump = LoadAnimations("jump");
		PlayerAttack = LoadAnimations("attack");
		PlayerDeath = LoadAnimations("death");
		PlayerSpawn = LoadAnimations("spawn");
		ret = CreateCol();
		currentTime = SDL_GetTicks();
		

	}

	else
	{
		LOG("player %s", result.description());
		return ret;
	}

	return ret;
}
 Animation j1Player::LoadAnimations(p2SString name)
{
	bool ret = true;
	pugi::xml_node p1_node = player_node.child("player1").child("animation");
	Animation anim_aux;

	SDL_Rect frameRect;
	for (pugi::xml_node frame = p1_node.child(name.GetString()).child("frame"); frame; frame = frame.next_sibling("frame"))
	{
		frameRect.x = frame.attribute("x").as_int();
		frameRect.y = frame.attribute("y").as_int();
		frameRect.w = frame.attribute("width").as_int();
		frameRect.h = frame.attribute("height").as_int();
		anim_aux.PushBack(frameRect);
		LOG("Animation: %s", name.GetString());
	}
	anim_aux.speed = 0.2f;
	return anim_aux;

}
inline bool j1Player::CreateCol()
{
	bool ret = false;
	offset.x = 3;
	offset.y = 0;
	SDL_Rect playerRect;
	playerRect.x = instantPos.x+offset.x;
	playerRect.y = instantPos.y;
	playerRect.w = player_node.child("player1").child("collider").attribute("w").as_int();
	playerRect.h = player_node.child("player1").child("collider").attribute("h").as_int();

	ColliderPlayer = App->collision->AddCollider(playerRect, COLLIDER_PLAYER, App->player1);
	if (ColliderPlayer != nullptr)
		ret = true;


	return ret;
}

bool j1Player::PreUpdate()
{
	//Player input-------------------------------------------------------------------
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE*/)
	{
		animState = AnimationState::ANIM_STATE_WALK;
		SpeedX = 0.5f;
		instantPos.x += SpeedX;
	}
		
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE*/)
	{
		animState = AnimationState::ANIM_STATE_WALK;
		SpeedX = -0.5f;
		instantPos.x += SpeedX;

	}


	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		animState = AnimationState::ANIM_STATE_IDLE;
		SpeedX = 0.0f;
	}

	
	//Gravity ------------------------------------------------------------------------
	if(activeGravity)
	{
		instantPos.y += /*initialPos.y +*/ App->map->gravity /**(SDL_GetTicks() - currentTime)*/;
	}

	

	/*instantPos.x = initialPos.x + SpeedX* ( SDL_GetTicks()-currentTime);*/

	
	return true;
}
bool j1Player::Update(float dt)
{
	SDL_Rect CurrentFrame;
	if (animState == AnimationState::ANIM_STATE_IDLE)
	{
	   CurrentFrame = PlayerIdle.GetCurrentFrame();
	}
	if (animState == AnimationState::ANIM_STATE_WALK)
	{
		CurrentFrame = PlayerWalk.GetCurrentFrame();
	}

	if(SpeedX<0.0f)
	App->render->Blit(ptexture,instantPos.x-CurrentFrame.w/2,instantPos.y-CurrentFrame.h,&CurrentFrame,SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(ptexture, instantPos.x - CurrentFrame.w / 2, instantPos.y - CurrentFrame.h, &CurrentFrame);

	ColliderPlayer->SetPos(instantPos.x + offset.x - CurrentFrame.w / 2, instantPos.y - CurrentFrame.h);
	ColliderPlayer->SetMeasurements(CurrentFrame.w-6, CurrentFrame.h);

	return true;
}

bool j1Player::PostUpdate()
{
	activeGravity = true;
	return true;
};
bool j1Player::CleanUp()
{
	App->tex->UnLoad(ptexture);
	ColliderPlayer->to_delete = true;
	return true;
}