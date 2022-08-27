import pathlib
from PIL import Image, ImageDraw

base_path = pathlib.Path(__file__).parent.resolve() / "images/"

backgrounds = {
    "discovered": Image.new("RGB", (400, 400), (144, 238, 144)),
    "cleared": Image.new("RGB", (400, 400), (255, 243, 109)),
}


def draw_house(image: Image) -> Image:
    draw = ImageDraw.Draw(image)
    draw.rectangle(((50, 150), (350, 400)), fill=(101, 67, 33))
    draw.polygon([(50, 150), (200, 50), (350, 150)], fill=(101, 67, 33))
    draw.rectangle(((160, 250), (240, 400)), fill=(188, 158, 130))
    draw.rectangle(((260, 250), (330, 320)), fill=(135, 206, 250))
    return image


def draw_castle(image: Image) -> Image:
    draw = ImageDraw.Draw(image)
    draw.rectangle(((10, 200), (390, 400)), fill=(92, 102, 106))
    draw.rectangle(((160, 250), (240, 400)), fill=(188, 158, 130))
    for i in range(5):
        start = 10 + (380 / 9) * (i * 2)
        end = 10 + (380 / 9) * (i * 2 + 1)
        draw.rectangle(((start, 150), (end, 200)), fill=(92, 102, 106))
    return image


def draw_character(image: Image) -> Image:
    draw = ImageDraw.Draw(image)
    draw.line(((100, 400), (200, 300)), fill=(199, 36, 177), width=10)
    draw.line(((300, 400), (200, 300)), fill=(199, 36, 177), width=10)
    draw.line(((200, 100), (200, 300)), fill=(199, 36, 177), width=10)
    draw.line(((100, 100), (200, 200)), fill=(199, 36, 177), width=10)
    draw.line(((300, 100), (200, 200)), fill=(199, 36, 177), width=10)
    draw.ellipse((150, 0, 250, 100), fill=(199, 36, 177))
    return image


def draw_sword(image: Image) -> Image:
    draw = ImageDraw.Draw(image)
    draw.line(((200, 0), (200, 400)), fill=(92, 102, 106), width=10)
    draw.line(((125, 350), (275, 350)), fill=(188, 158, 130), width=10)
    return image


locations = {
    "": lambda x: x,
    "house": draw_house,
    "castle": draw_castle,
}
users = {"": lambda x: x, "character": draw_character}


def crop_image(
    image: Image, left: float, upper: float, right: float, lower: float
) -> Image:
    "Crops a given image. Each number is the percentage you want to remove."
    width, height = image.size
    return image.crop(
        (width * left, upper * height, (1 - right) * width, (1 - lower) * height)
    )


def create_image(background: str, location: str, user: str):
    back_img = backgrounds[background].copy()
    with_loc = locations[location](back_img)
    user_loc = users[user](with_loc)
    user_loc.save(base_path / f"{background}-{location}-{user}.png")


if __name__ == "__main__":
    # First move over images that are not changed
    Image.new("RGB", (400, 400), (34, 139, 34)).save(base_path / "undiscovered.png")
    sword = draw_sword(
        Image.new("RGBA", (400, 400), (255, 0, 0, 0)),
    )
    crop_image(sword, 0.0, 0, 0.0, 0).save(base_path / "sword.png")
    character = draw_character(
        Image.new("RGBA", (400, 400), (255, 0, 0, 0)),
    )
    crop_image(character, 0.25, 0, 0.25, 0).save(base_path / "character.png")
    # Now generate images
    for back in backgrounds:
        for loc in locations:
            for use in users:
                create_image(back, loc, use)
