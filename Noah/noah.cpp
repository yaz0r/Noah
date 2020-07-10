bool noahInit(int argc, char* argv[]);
bool noahFrame();

int main(int argc, char* argv[])
{
    noahInit(argc, argv);

    while (!noahFrame())
    {
    }

    return 0;
}
