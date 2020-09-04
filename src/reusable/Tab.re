module Styles = {
  open Css;

  let container =
    style([
      backgroundColor(Colors.white),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(10), Css.rgba(0, 0, 0, 0.08))),
      Media.mobile([margin2(~h=`px(-15), ~v=`zero)]),
    ]);
  let header =
    style([
      backgroundColor(Colors.white),
      padding2(~v=`zero, ~h=`px(20)),
      borderBottom(`px(1), `solid, Colors.gray4),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(10), Css.rgba(0, 0, 0, 0.08))),
      Media.mobile([overflow(`auto), padding2(~v=`px(5), ~h=`px(10))]),
    ]);

  let buttonContainer = active =>
    style([
      height(`px(40)),
      display(`inlineFlex),
      justifyContent(`center),
      alignItems(`center),
      cursor(`pointer),
      padding2(~v=Spacing.md, ~h=`px(20)),
      borderBottom(`pxFloat(1.5), `solid, active ? Colors.bandBlue : Colors.white),
      textShadow(Shadow.text(~blur=`pxFloat(active ? 1. : 0.), Colors.bandBlue)),
      Media.mobile([whiteSpace(`nowrap), borderColor(Colors.white)]),
    ]);

  let childrenContainer =
    style([
      backgroundColor(Colors.blueGray1),
      Media.mobile([padding2(~h=`px(16), ~v=`zero), minHeight(`px(200))]),
    ]);
};

let button = (~name, ~route, ~active) => {
  <Link key=name isTab=true className={Styles.buttonContainer(active)} route>
    <Text
      value=name
      weight=Text.Regular
      size=Text.Md
      color={active ? Colors.bandBlue : Colors.gray6}
    />
  </Link>;
};

type t = {
  name: string,
  route: Route.t,
};

[@react.component]
let make = (~tabs: array(t), ~currentRoute, ~children) => {
  <div className=Styles.container>
    <div className=Styles.header>
      <Row>
        {tabs
         ->Belt.Array.map(({name, route}) =>
             button(~name, ~route, ~active=route == currentRoute)
           )
         ->React.array}
      </Row>
    </div>
    <div className=Styles.childrenContainer> children </div>
  </div>;
};
