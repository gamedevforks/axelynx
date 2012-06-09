﻿#include <axelynx/axelynx.h>


using namespace axelynx;

int main()
{
        Engine *eng = Engine::Init();

        Window *wnd = eng->AddWindow(800,600,32,WM_WINDOWED,4);

		wnd->VSync(false);

        //TODO: Place your resource loading and scene creating code here

        Scene *s = eng->AddScene();
        Camera *cam = s->AddCamera();
		cam->ClearColor(0.2,0.3,0.4);

		Canvas *c = eng->GetCanvas();

		//cam->SetDrawMode(Camera::DM_WIREFRAME);

        Surface *cube = StandartSurfaces::Grid(256,256,16,16,true);
        Entity *body = s->Add(cube);
		//body->SetScale(256);

		Texture *diffuse = eng->LoadTexture(L"../../../../samples/media/solider.pcx",Texture::Desc().Anisotropic(16.0));
		MorfedMesh *mesh = eng->LoadMorfedMesh(L"../../../../samples/media/solider.md2");

		Shader *shader = StandartShaders::Render::MorfedMeshTexturing();

		const int COUNT_TEAPOTS = 1000;

		Entity* ents[COUNT_TEAPOTS];
		quat rots[COUNT_TEAPOTS];

		for(int i=0;i<COUNT_TEAPOTS;++i)
		{
			ents[i] = s->Add(mesh);
			ents[i]->SetShader(shader);
			ents[i]->SetTexture(diffuse);
			ents[i]->SetPosition(rnd(-128,128),rnd(2,2),rnd(-128,128));
			ents[i]->SetOrientation(0,rnd(360),0);

			rots[i] = quat(vec3(0,rnd(-0.5,0.5),0));
		}

		vec3 transl;

		float time =0;

		bool wire = false;

		
		Font::Desc desc(L"../../../../samples/media/CODE2000.ttf");	
		desc.size = 16;
		Font *fnt = eng->LoadFont(desc);
		c->SetFont(fnt);

        while(wnd->isRunning())
        {
			for(int i=0;i<COUNT_TEAPOTS;++i)
			{
				ents[i]->Turn(rots[i]);
			}

			if(KeyHit('R'))
			{
				wire = ! wire;
				cam->SetDrawMode(wire?Camera::DM_WIREFRAME:Camera::DM_SOLID);
			};

            //TODO: Place your rendering code here
			float dt = Timer::Delta();
			time += dt*0.001;


			quat qcam(vec3(float(MouseY()-300)*0.2f,float(MouseX()-400)*0.2f,0));
			cam->SetOrientation(qcam);

			vec3 move;

			if(KeyDown('W'))
				move.z -= 0.1;
			if(KeyDown('S'))
				move.z += 0.1;
			if(KeyDown('A'))
				move.x -= 0.1;
			if(KeyDown('D'))
				move.x += 0.1;

			move = cam->GetOrientation().ToMat3() * move;
			transl += move;

			transl *= 0.9;

			cam->Translate(transl * dt * 0.1);

			s->Render();
			c->SetBlendMode(BM_ALPHA);
			c->SetColor(1,1,1);
			c->SetAlpha(1);
			c->SetRotate(0);
			c->SetScale(1);
			c->SetPosition(10,10);
			c->Print(L"FPS:%d",eng->GetStatistics()->GetFPS());
			//fnt->Draw(c,L"trtrt");
			c->SetBlendMode(BM_NONE);
            wnd->Flip();
        }
        //TODO: Place your deinitalize code here
        eng->Free();
}