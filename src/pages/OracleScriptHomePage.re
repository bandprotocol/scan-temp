module Styles = {
  open Css;

  let vFlex = style([display(`flex), flexDirection(`row), alignItems(`center)]);

  let seperatedLine =
    style([
      width(`px(13)),
      height(`px(1)),
      marginLeft(`px(10)),
      marginRight(`px(10)),
      backgroundColor(Colors.gray7),
    ]);

  let logo = style([width(`px(50)), marginRight(`px(10))]);

  let addressContainer = style([width(`px(320))]);

  let fullWidth = style([width(`percent(100.0)), display(`flex)]);
};

[@react.component]
let make = () =>
  {
    let (page, setPage) = React.useState(_ => 1);
    let pageSize = 10;

    let oracleScriptsCountSub = OracleScriptSub.count();
    let oracleScriptsSub = OracleScriptSub.getList(~pageSize, ~page, ());

    let%Sub oracleScriptsCount = oracleScriptsCountSub;
    let%Sub oracleScripts = oracleScriptsSub;

    let pageCount = Page.getPageCount(oracleScriptsCount, pageSize);

    <>
      <Row>
        <Col>
          <div className=Styles.vFlex>
            <img src=Images.oracleScriptLogo className=Styles.logo />
            <Text
              value="ALL ORACLE SCRIPTS"
              weight=Text.Medium
              size=Text.Md
              spacing={Text.Em(0.06)}
              height={Text.Px(15)}
              nowrap=true
              color=Colors.gray7
              block=true
            />
            <div className=Styles.seperatedLine />
            <Text
              value={oracleScriptsCount->string_of_int ++ " In total"}
              size=Text.Md
              weight=Text.Thin
              spacing={Text.Em(0.06)}
              color=Colors.gray7
              nowrap=true
            />
          </div>
        </Col>
      </Row>
      <VSpacing size=Spacing.xl />
      <>
        <THead>
          <Row>
            <Col> <HSpacing size=Spacing.lg /> </Col>
            <Col size=1.5>
              <div className=TElement.Styles.hashContainer>
                <Text
                  block=true
                  value="NAME"
                  size=Text.Sm
                  weight=Text.Semibold
                  color=Colors.gray5
                  spacing={Text.Em(0.1)}
                />
              </div>
            </Col>
            <Col size=1.3>
              <Text
                block=true
                value="TIMESTAMP"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                spacing={Text.Em(0.1)}
              />
            </Col>
            <Col size=2.0>
              <Text
                block=true
                value="OWNER"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                spacing={Text.Em(0.1)}
              />
            </Col>
            // <Col size=1.35>
            //   <Text
            //     block=true
            //     value="DATA SOURCES"
            //     size=Text.Sm
            //     weight=Text.Semibold
            //     color=Colors.gray5
            //     spacing={Text.Em(0.1)}
            //   />
            // </Col>
            <Col> <HSpacing size=Spacing.lg /> </Col>
          </Row>
        </THead>
        {oracleScripts
         ->Belt.Array.map(({id, name, timestamp, owner}) => {
             <TBody key={id |> ID.OracleScript.toString}>
               <div className=Styles.fullWidth>
                 <Row alignItems=`baseline>
                   <Col> <HSpacing size=Spacing.lg /> </Col>
                   <Col size=1.5>
                     <TElement elementType={TElement.OracleScript(id, name)} />
                   </Col>
                   <Col size=1.3> <TElement elementType={timestamp->TElement.Timestamp} /> </Col>
                   <Col size=2.0>
                     <div className=Styles.addressContainer>
                       <TElement elementType={owner->TElement.Address} />
                     </div>
                   </Col>
                   //  <Col size=1.35>
                   //    <TElement elementType={relatedDataSources->TElement.RelatedDataSources} />
                   //  </Col>
                   <Col> <HSpacing size=Spacing.lg /> </Col>
                 </Row>
               </div>
             </TBody>
           })
         ->React.array}
      </>
      <VSpacing size=Spacing.lg />
      <Pagination currentPage=page pageCount onPageChange={newPage => setPage(_ => newPage)} />
      <VSpacing size=Spacing.lg />
    </>
    |> Sub.resolve;
  }
  |> Sub.default(_, React.null);
