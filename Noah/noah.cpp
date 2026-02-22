bool noahInit(int argc, char* argv[]);
bool noahFrame();
void noahExit();

#if defined(SHIPPING_BUILD) && defined(WIN32)
int WinMain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    noahInit(argc, argv);

    noahExit();

    return 0;
}
