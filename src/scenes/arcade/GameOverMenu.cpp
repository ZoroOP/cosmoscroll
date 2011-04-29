#include <SFML/Network.hpp>

#include "GameOverMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "entities/EntityManager.hpp"
#include "entities/PlayerShip.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "md5/md5.hpp"


GameOverMenu::GameOverMenu()
{
	LoadBitmapFont("data/images/gui/mono12-black.png", 10, 10);
	SetTitle(I18n::t("menu.gameover.title"));

	score_ = 0;
	lab_result_ = new gui::Label(this, "", 120, 120);
	lab_result_->SetSize(30);

	lab_pseudo_ = new gui::Label(this, I18n::t("menu.gameover.pseudo"), 100, 200);
	txt_pseudo_ = new gui::TextBox(this, 210, 200, 30);
	txt_pseudo_->SetCallbackID(3);

	but_commit_ = new CosmoButton(this, I18n::t("menu.submit"), 210, 240);
	but_commit_->SetCallbackID(3);

	but_send_score_ = new CosmoButton(this, I18n::t("menu.gameover.send_score"), 210, 240);
	but_send_score_->SetCallbackID(0);

	(new CosmoButton(this, I18n::t("menu.gameover.play_again"), 210, 290))->SetCallbackID(1);
	(new CosmoButton(this, I18n::t("menu.back_main_menu"), 210, 340))->SetCallbackID(2);
}


void GameOverMenu::OnFocus()
{
	BaseMenu::OnFocus();
	EntityManager& entities = EntityManager::GetInstance();

	score_ = entities.GetPlayerShip()->GetPoints();

	std::wstring text;
	if (score_ > entities.GetArcadeRecord())
	{
		entities.UpdateArcadeRecord();
		text = str_sprintf(I18n::t("menu.gameover.new_record").c_str(), score_);
	}
	else
	{
		text = str_sprintf(I18n::t("menu.gameover.no_record").c_str(), score_);
	}
	lab_result_->SetSize(30);
	lab_result_->SetText(text);

	but_send_score_->SetVisible(true);
	FocusWidget(but_send_score_);
	lab_pseudo_->SetVisible(false);
	txt_pseudo_->SetVisible(false);
	but_commit_->SetVisible(false);
}


void GameOverMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			but_commit_->SetVisible(true);
			txt_pseudo_->SetVisible(true);
			lab_pseudo_->SetVisible(true);
			FocusWidget(txt_pseudo_);
			but_send_score_->SetVisible(false);
			break;
		case 1:
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		case 2:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
		case 3:
			if (EntityManager::GetInstance().GetPlayerShip()->HasCheated())
			{
				lab_result_->SetSize(20);
				lab_result_->SetText(I18n::t("menu.gameover.error_cheat"));
			}
			else if (!Game::GetInstance().IsPure())
			{
				lab_result_->SetSize(20);
				lab_result_->SetText(I18n::t("menu.gameover.error_altered_res"));
			}
			else
			{
				UploadScore();
			}
			break;
	}
}


void GameOverMenu::UploadScore()
{
	std::string str_name = txt_pseudo_->GetText();
	std::string str_score = str_sprintf("%d", score_);

	// COSMO_SERVER_KEY is a salt, producing a different salted key for each couple (name, score)
	MD5 key(str_name + str_score + COSMO_SERVER_KEY);

	// Connect to cosmoscroll scores server
	sf::Http server;
	server.SetHost(COSMO_SERVER_HOSTNAME);

	// Submit the score
	sf::Http::Request request;
	request.SetMethod(sf::Http::Request::Post);
	request.SetURI(COSMO_SERVER_URI);
	std::string body = "name=" + str_name
	                 + "&score=" + str_score
	                 + "&key=" + key.GetHash()
	                 + "&version=" + COSMOSCROLL_VERSION;
	request.SetBody(body);

	// Send it and get the response returned by the server
	sf::Http::Response response = server.SendRequest(request);
	switch (response.GetStatus())
	{
		case sf::Http::Response::Ok:
			Game::GetInstance().SetNextScene(Game::SC_BestScoresMenu);
			break;
		case sf::Http::Response::ConnectionFailed:
			lab_result_->SetText("Couldn't connect to CosmoScroll server");
			break;
		default:
			lab_result_->SetText(response.GetBody());
			break;
	}
}
