bool noahInit(int argc, char* argv[]);
bool noahFrame();
void noahExit();

int main(int argc, char* argv[])
{
    noahInit(argc, argv);

    while (!noahFrame())
    {
    }

    noahExit();

    return 0;
}
