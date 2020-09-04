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

  let fullWidth = style([width(`percent(100.0)), display(`flex)]);

  let progressBarContainer = style([maxWidth(`px(300))]);

  let resolveStatusContainer = style([justifyContent(`center)]);
};

[@react.component]
let make = () =>
  {
    let (page, setPage) = React.useState(_ => 1);
    let pageSize = 10;

    let requestCountSub = RequestSub.count();
    let requestsSub = RequestSub.getList(~pageSize, ~page, ());

    let%Sub requestCount = requestCountSub;
    let%Sub requests = requestsSub;

    let pageCount = Page.getPageCount(requestCount, pageSize);

    <>
      <Row>
        <Col>
          <div className=Styles.vFlex>
            <img src=Images.requestLogo className=Styles.logo />
            <Text
              value="ALL REQUESTS"
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
              value={requestCount->string_of_int ++ " In total"}
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
            <Col size=0.5>
              <Text
                block=true
                value="REQUEST ID"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                spacing={Text.Em(0.1)}
              />
            </Col>
            <Col size=1.0>
              <Text
                block=true
                value="TIMESTAMP"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                spacing={Text.Em(0.1)}
              />
            </Col>
            <Col size=1.15>
              <Text
                block=true
                value="ORACLE SCRIPTS"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                spacing={Text.Em(0.1)}
              />
            </Col>
            <Col size=1.9>
              <Text
                block=true
                value="REPORT STATUS"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                spacing={Text.Em(0.1)}
              />
            </Col>
            <Col size=0.72 justifyContent=Col.End>
              <Text
                block=true
                value="RESOLVE STATUS"
                size=Text.Sm
                weight=Text.Semibold
                color=Colors.gray5
                align=Text.Right
                spacing={Text.Em(0.1)}
              />
            </Col>
            <Col> <HSpacing size=Spacing.xl /> </Col>
          </Row>
        </THead>
        {requests
         ->Belt_Array.map(
             (
               {
                 id,
                 transaction,
                 oracleScript,
                 requestedValidators,
                 minCount,
                 reports,
                 resolveStatus,
               },
             ) => {
             <TBody key={id |> ID.Request.toString}>
               <div className=Styles.fullWidth>
                 <Row minHeight={`px(35)}>
                   <Col> <HSpacing size=Spacing.lg /> </Col>
                   <Col size=0.5> <TElement elementType={TElement.Request(id)} /> </Col>
                   <Col size=1.0>
                     <TElement elementType={transaction.block.timestamp->TElement.Timestamp} />
                   </Col>
                   <Col size=1.15>
                     <TElement
                       elementType={
                         TElement.OracleScript(oracleScript.oracleScriptID, oracleScript.name)
                       }
                     />
                   </Col>
                   <Col size=1.9>
                     <div className=Styles.progressBarContainer>
                       <ProgressBar
                         reportedValidators={reports |> Belt_Array.size}
                         minimumValidators=minCount
                         requestValidators={requestedValidators |> Belt_Array.size}
                       />
                     </div>
                   </Col>
                   <Col size=0.72 justifyContent=Col.End>
                     <TElement elementType={resolveStatus->TElement.RequestStatus} />
                   </Col>
                   <Col> <HSpacing size=Spacing.xl /> </Col>
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
