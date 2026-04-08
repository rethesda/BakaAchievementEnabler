namespace Hooks
{
	namespace hkShowUsingConsoleMayDisableAchievements
	{
		static void Install()
		{
			static REL::Relocation target{ REL::ID(113625) };
			static constexpr auto  TARGET_ADDR{ 0xE9 };
			static constexpr auto  TARGET_RETN{ 0xEE };
			static constexpr auto  TARGET_FILL{ TARGET_RETN - TARGET_ADDR };
			target.write_fill<TARGET_ADDR>(REL::NOP, TARGET_FILL);
		}
	}

	namespace hkAddAchievement
	{
		static void Install()
		{
			static REL::Relocation target{ REL::ID(117359) };
			static constexpr auto  TARGET_ADDR{ 0x082 };
			static constexpr auto  TARGET_RETN{ 0x104 };
			static constexpr auto  TARGET_FILL{ TARGET_RETN - TARGET_ADDR };
			target.write_fill<TARGET_ADDR>(REL::NOP, TARGET_FILL);
		}
	}

	namespace hkNoModdedTag
	{
		static void Install()
		{
			static REL::Relocation target{ REL::ID(114443) };
			static constexpr auto  TARGET_ADDR{ 0xAE };
			static constexpr auto  TARGET_RETN{ 0xD4 };
			static constexpr auto  TARGET_FILL{ TARGET_RETN - TARGET_ADDR };
			target.write_fill<TARGET_ADDR>(REL::NOP, TARGET_FILL);
		}
	}

	namespace hkConfirmNewWithModsCallback
	{
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(87568) };
			target.write<0x39, std::uint8_t>(0x02);
		}
	}

	class hkShowLoadVanillaSaveWithMods
	{
	private:
		static void ShowLoadVanillaSaveWithMods()
		{
			static REL::Relocation<std::uint32_t*> dword{ REL::ID(896839) };
			(*dword.get()) &= ~2;

			static REL::Relocation<void (*)(void*, void*, std::int32_t, std::int32_t, void*)> func{ REL::ID(98666) };
			return func(nullptr, nullptr, 0, 0, nullptr);
		}

		inline static REL::THook _Hook{ REL::ID(98666), 0x167, ShowLoadVanillaSaveWithMods };
	};

	static void Install()
	{
		// Disable "$UsingConsoleMayDisableAchievements" message
		hkShowUsingConsoleMayDisableAchievements::Install();

		// Disable AddAchievement checks
		hkAddAchievement::Install();

		// Disable modded tag
		hkNoModdedTag::Install();

		// Disable ConfirmNewWithModsCallback
		hkConfirmNewWithModsCallback::Install();
	}
}

namespace
{
	void MessageCallback(SFSE::MessagingInterface::Message* a_msg) noexcept
	{
		switch (a_msg->type)
		{
		case SFSE::MessagingInterface::kPostLoad:
			Hooks::Install();
			break;
		default:
			break;
		}
	}
}

SFSE_PLUGIN_LOAD(const SFSE::LoadInterface* a_sfse)
{
	SFSE::Init(a_sfse, { .trampoline = true, .trampolineSize = 16 });
	SFSE::GetMessagingInterface()->RegisterListener(MessageCallback);
	return true;
}
